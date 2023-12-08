#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <mpi.h>
#include <ctime>
using namespace std;

void timestamp();

// Função para ler o grafo a partir do arquivo de entrada
vector<vector<int>> LerGrafo(const string& nomeArquivo, int& numVertices) {
    ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    vector<vector<int>> grafo(numVertices, vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // O grafo é não direcionado
    }

    arquivo.close();

    return grafo;
}

void EncontraTodosCliquesMax(vector<vector<int>>& grafo, vector<int>& candidatos, vector<int>& cliqueAtual, vector<int>& cliqueMaximo) {
    if (candidatos.empty()) {
        // Verifica se o clique atual é um clique (todos os vértices conectados entre si)
        bool ehClique = true;
        for (size_t i = 0; i < cliqueAtual.size() - 1 && ehClique; ++i) {
            for (size_t j = i + 1; j < cliqueAtual.size() && ehClique; ++j) {
                if (grafo[cliqueAtual[i]][cliqueAtual[j]] == 0) {
                    ehClique = false;
                }
            }
        }

        if (ehClique && cliqueAtual.size() > cliqueMaximo.size()) {
            cliqueMaximo = cliqueAtual;
        }

        return;
    }

    // usa uma cópia dos candidatos
    vector<int> candidatosCopia = candidatos;

    while (!candidatosCopia.empty()) {
        int v = candidatosCopia.back();
        candidatosCopia.pop_back();

        // Inicia com o vértice atual
        cliqueAtual.push_back(v);

        // atualiza a lista de candidatos
        vector<int> novosCandidatos;
        for (size_t i = 0; i < candidatosCopia.size(); ++i) {
            int u = candidatosCopia[i];
            if (grafo[v][u] == 1) {
                novosCandidatos.push_back(u);
            }
        }

        EncontraTodosCliquesMax(grafo, novosCandidatos, cliqueAtual, cliqueMaximo);
        cliqueAtual.pop_back();
    }
}


int main(int argc, char** argv) {
    timestamp();
    
    MPI_Init(&argc, &argv);

    int numProcessos, processoAtual;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcessos);
    MPI_Comm_rank(MPI_COMM_WORLD, &processoAtual);

    int numVertices;
    vector<vector<int>> grafo;
    vector<int> candidatos;
    vector<int> cliqueAtual;
    vector<int> cliqueMaximo;

    if (processoAtual == 0) {
        grafo = LerGrafo("amostra_grafos/grafo_120.txt", numVertices);
        numVertices = grafo.size();
        for (int i = 0; i < numVertices; ++i) {
            candidatos.push_back(i);
        }
    }

    // Broadcast do número de vértices para todos os processos
    MPI_Bcast(&numVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Divide os candidatos entre os processos
    int tamanhoBloco = candidatos.size() / numProcessos;
    int inicio = processoAtual * tamanhoBloco;
    int fim = (processoAtual == numProcessos - 1) ? candidatos.size() : inicio + tamanhoBloco;
    vector<int> candidatosLocais(candidatos.begin() + inicio, candidatos.begin() + fim);

    // Broadcast dos candidatos locais para todos os processos
    MPI_Bcast(candidatosLocais.data(), candidatosLocais.size(), MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo executa a função EncontraTodosCliquesMax com seus candidatos locais
    EncontraTodosCliquesMax(grafo, candidatosLocais, cliqueAtual, cliqueMaximo);

    // Combinar os resultados - processo 0 recebe os resultados de outros processos
    if (processoAtual == 0) {
        for (int i = 1; i < numProcessos; ++i) {
            vector<int> cliqueMaximoRemoto;
            MPI_Recv(cliqueMaximoRemoto.data(), cliqueMaximoRemoto.size(), MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (cliqueMaximoRemoto.size() > cliqueMaximo.size()) {
                cliqueMaximo = cliqueMaximoRemoto;
            }
        }
    } else {
        // Outros processos enviam seus resultados para o processo 0
        MPI_Send(cliqueMaximo.data(), cliqueMaximo.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Processo 0 exibe o resultado final
    if (processoAtual == 0) {
        sort(cliqueMaximo.begin(), cliqueMaximo.end(), [](auto& i, auto& j) { return i < j; });

        cout << "Clique máxima encontrada: [";
        for (size_t i = 0; i < cliqueMaximo.size(); ++i) {
            cout << cliqueMaximo[i] + 1;
            if (i < cliqueMaximo.size() - 1) {
                cout << ", ";
            }
        }
        cout << "]" << endl;
    }

    MPI_Finalize();

    timestamp();

    return 0;
}

void timestamp() {
   char time_buffer[40];
   time_t now = time(nullptr);
   tm *tm = localtime(&now);
   strftime(time_buffer, sizeof(time_buffer), "%I:%M:%S", tm);
   std::cout << time_buffer << std::endl;
}
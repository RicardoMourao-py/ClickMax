#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <omp.h>
using namespace std;

// Função para ler o grafo a partir do arquivo de entrada
vector<vector<int>> LerGrafo(const string& nomeArquivo, int& numVertices) {
    ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    vector<vector<int>> grafo(numVertices, vector<int>(numVertices, 0));

    #pragma omp parallel for
    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        #pragma omp critical
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
            #pragma omp parallel for
            for (size_t j = i + 1; j < cliqueAtual.size(); ++j) {
                // Use a cláusula if para verificar ehClique dentro do loop
                if (ehClique) {
                    if (grafo[cliqueAtual[i]][cliqueAtual[j]] == 0) {
                        #pragma omp atomic write
                        ehClique = false;  // Sinaliza que ehClique deve ser definido como falso
                    }
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


int main() {
    int numVertices;
    vector<vector<int>> grafo;
    vector<int> candidatos;
    vector<int> cliqueAtual;
    vector<int> cliqueMaximo;

    grafo = LerGrafo("grafo.txt", numVertices);
    numVertices = grafo.size();
    for (int i = 0; i < numVertices; ++i) {
        candidatos.push_back(i);
    }

    EncontraTodosCliquesMax(grafo, candidatos, cliqueAtual, cliqueMaximo);
    sort(cliqueMaximo.begin(),cliqueMaximo.end(), [](auto& i, auto& j){return i < j;});

    cout << "Clique máxima encontrada: [";
    for (size_t i = 0; i < cliqueMaximo.size(); ++i) {
        cout << cliqueMaximo[i] + 1;
        if (i < cliqueMaximo.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;

    return 0;
}
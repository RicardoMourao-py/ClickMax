#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;


bool CompararPorGrau(const pair<int, int>& a, const pair<int, int>& b) {
    return a.second < b.second;
}

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

vector<int> EncontrarCliqueMaxima(vector<vector<int>>& grafo, int numVertices) {
    vector<int> cliqueMaxima;
    vector<pair<int, int>> graus;  // Armazena pares (nó, grau)

    // Calcular grau de cada nó
    for (int i = 0; i < numVertices; ++i) {
        int grau = 0;
        for (int j = 0; j < numVertices; ++j) {
            grau += grafo[i][j];
        }
        graus.push_back({i, grau});
    }

    // Ordenar nós por grau em ordem decrescente
    sort(graus.begin(), graus.end(), CompararPorGrau);

    while (!graus.empty()) {
        int v = graus.back().first;
        graus.pop_back();

        bool podeAdicionar = true;

        for (int u : cliqueMaxima) {
            if (grafo[u][v] == 0) {
                podeAdicionar = false;
                break;
            }
        }

        if (podeAdicionar) {
            cliqueMaxima.push_back(v);
            vector<pair<int, int>> novosGraus;

            // Atualizar graus removendo nós não adjacentes a v
            for (auto& par : graus) {
                int u = par.first;

                if (grafo[u][v] == 1) {
                    novosGraus.push_back(par);
                }
            }

            graus = novosGraus;
        }
    }

    return cliqueMaxima;
}


int main() {
    int numVertices;
    vector<vector<int>> grafo;

    grafo = LerGrafo("grafo.txt", numVertices);
    vector<int> cliqueMaximo = EncontrarCliqueMaxima(grafo, numVertices);
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
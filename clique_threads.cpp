#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <omp.h>
using namespace std;


bool CompararPorGrau(const pair<int, int>& a, const pair<int, int>& b) {
    return a.second > b.second;
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

    #pragma omp parallel for
    for (int i = 0; i < numVertices; ++i) {
        int grau = 0;
        for (int j = 0; j < numVertices; ++j) {
            grau += grafo[i][j];
        }

        #pragma omp critical
        graus.push_back({i, grau});
    }

    // Ordenar nós por grau em ordem decrescente
    sort(graus.begin(), graus.end(), CompararPorGrau);

    #pragma omp parallel
    {
      while (!graus.empty()) {
          int v;

          #pragma omp critical
          {
              v = graus.back().first;
              graus.pop_back();
          }

          bool podeAdicionar = true;

          #pragma omp for
          for (size_t i = 0; i < cliqueMaxima.size(); ++i) {
              int u = cliqueMaxima[i];
              if (grafo[u][v] == 0) {
                  podeAdicionar = false;
              }
          }

          #pragma omp critical
          {
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

    cout << "Abordagem Theads com Clique de Tamanho: "<< cliqueMaximo.size() <<" Maximal Clique: ";
    for (int v : cliqueMaximo) {
        cout << v+1 << " ";
    }
    cout << endl;

    return 0;
}

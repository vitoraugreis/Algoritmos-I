#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Grafo.hpp"

using namespace std;

int main() {
    vector<string> centros_urbanos;     // Vetor que armazena os nomes dos centros urbanos.
    unordered_map<string, int> mapa;    // Mapa que armazena o indice do centro urbano do vetor. Usado para preencher o grafo.

    int centros_registrados = 0;

    int qntd_centros_urbanos, qntd_estradas;
    cin >> qntd_centros_urbanos >> qntd_estradas;

    // Um grafo será usado para representar o império.
    Grafo grafo(qntd_centros_urbanos);
    
    // Preenchimento do grafo.
    for (int i = 0; i<qntd_estradas; i++) {
        string centro_origem, centro_destino;
        cin >> centro_origem >> centro_destino;
        if (mapa.find(centro_origem) == mapa.end()) {
            mapa[centro_origem] = centros_registrados;
            centros_registrados++;
            centros_urbanos.push_back(centro_origem);
        }
        if (mapa.find(centro_destino) == mapa.end()) {
            mapa[centro_destino] = centros_registrados;
            centros_registrados++;
            centros_urbanos.push_back(centro_destino);
        }
        grafo.adicionarAresta(mapa[centro_origem], mapa[centro_destino]);
    }

    // Definição e impressão da capital.
    grafo.definirCapital();
    cout << centros_urbanos[grafo.getCapital()] << endl;

    // Definição e impressão dos batalhões secundários.
    grafo.definirBatalhoes();
    vector<int> batalhoes = grafo.getBatalhoes();
    cout << batalhoes.size() << endl;
    for (auto it = batalhoes.begin(); it != batalhoes.end(); it++) {
        cout << centros_urbanos[*it] << endl;
    }

    // Definição e impressão das rotas.
    grafo.definirRotas();
    vector<vector<int>> rotas = grafo.getRotas();
    cout << rotas.size() << endl;
    for (auto it = rotas.begin(); it != rotas.end(); it++) {
        for (auto jt = (*it).begin(); jt != (*it).end(); jt++) {
            cout << centros_urbanos[*jt] << ' ';
        }
        cout << endl;
    }

    return 0;
}
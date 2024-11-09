#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Grafo.hpp"

using namespace std;

int main() {
    vector<string> centros_urbanos;
    unordered_map<string, int> mapa;
    int centros_registrados = 0;

    int qntd_centros_urbanos, qntd_estradas;
    cin >> qntd_centros_urbanos >> qntd_estradas;

    Grafo grafo(qntd_centros_urbanos);
    
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

    grafo.definirCapital();
    cout << centros_urbanos[grafo.getCapital()] << endl;

    grafo.definirBatalhoes();
    vector<int> batalhoes = grafo.getBatalhoes();
    cout << batalhoes.size() << endl;
    for (auto it = batalhoes.begin(); it != batalhoes.end(); it++) {
        cout << centros_urbanos[*it] << endl;
    }

    grafo.definirRotas();
    vector<vector<int>> rotas = grafo.getRotas();

    for (auto it = rotas.begin(); it != rotas.end(); it++) {
        for (auto jt = (*it).begin(); jt != (*it).end(); jt++) {
            cout << centros_urbanos[*jt] << ' ';
        }
        cout << endl;
    }
}
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
    vector<int> v = grafo.getDistanciasAteCapital();
    for (int i = 0; i<qntd_centros_urbanos; i++) {
        cout << centros_urbanos[i] << ": " << v[i] << endl;
    }
    
    //cout << grafo.kosaraju() << endl;
}
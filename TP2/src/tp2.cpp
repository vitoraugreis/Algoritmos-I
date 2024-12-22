#include "Grafo.hpp"
#include <iostream>

using namespace std;

int main() {
    int v, e;
    cin >> v >> e;

    Grafo siderurgica(v);

    for (int i = 0; i<v; i++) {
        int id, demanda;
        cin >> id >> demanda;
        siderurgica.adicionarVertice(id, demanda);
    }

    for (int i = 0; i<e; i++) {
        int origem, destino, capacidade;
        cin >> origem >> destino >> capacidade;
        siderurgica.adicionarAresta(origem, destino, capacidade);
    }

    cout << siderurgica.definirEnergiaTotal() << endl;
    cout << siderurgica.definirEnergiaNaoAtendida() << endl;
    return 0;
}
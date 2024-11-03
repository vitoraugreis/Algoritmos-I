#include "Grafo.hpp"

Grafo::Grafo(int num_vertices) {
    this->num_vertices = num_vertices;
    this->matriz_adj = new int*[num_vertices];

    for (int i = 0; i<num_vertices; i++) {
        this->matriz_adj[i] = new int[num_vertices];
        for (int j = 0; j<num_vertices; j++) {
            this->matriz_adj[i][j] = 0;
        }
    }
}

Grafo::~Grafo() {
    for (int i = 0; i<this->num_vertices; i++) {
        delete[] this->matriz_adj[i];
    }
    delete[] this->matriz_adj;
}

void Grafo::adicionarAresta(int origem, int destino) {
    this->matriz_adj[origem][destino] = 1;
}

void Grafo::imprimirMatrizAdj() {
    for (int i = 0; i<this->num_vertices; i++) {
        for (int j = 0; j<this->num_vertices; j++) {
            std::cout << this->matriz_adj[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}
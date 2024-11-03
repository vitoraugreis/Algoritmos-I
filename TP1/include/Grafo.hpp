#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>

class Grafo {
    public:
        Grafo(int num_vertices);
        ~Grafo();
        void adicionarAresta(int origem, int destino);
        void imprimirMatrizAdj();
    private:
        int** matriz_adj;
        int num_vertices;
};

#endif
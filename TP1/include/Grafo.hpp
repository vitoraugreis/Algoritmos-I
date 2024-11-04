#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
#include <utility>
#include <vector>
#include <queue>

class Grafo {
    public:
        Grafo(int num_vertices);
        ~Grafo();
        void adicionarAresta(int origem, int destino);
        int definirCapital();
        void imprimirMatrizAdj();
    private:
        int** matriz_adj;
        int num_vertices;
        int bfs(int origem);
};

#endif
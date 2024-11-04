#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <list>

class Grafo {
    public:
        Grafo(int num_vertices);
        ~Grafo();
        void adicionarAresta(int origem, int destino);
        int definirCapital();
        void imprimirListaAdj();
    private:
        int num_vertices;
        std::vector<std::list<int>> lista_adj;
        int bfs(int origem);
};

#endif
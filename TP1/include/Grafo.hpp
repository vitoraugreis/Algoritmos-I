#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <list>
#include <stack>

using namespace std;

class Grafo {
    public:
        Grafo(int num_vertices);
        ~Grafo();
        void adicionarAresta(int origem, int destino);
        void definirCapital();
        void definirBatalhoes();

        int getCapital();
        vector<int> getBatalhoes();
        vector<int> getDistanciasAteCapital();
        void imprimirListaAdj();
    private:
        void bfs(int origem, int* distancia_total);
        void dfs_padrao(vector<list<int>> &grafo, int vertice, vector<bool> &visitados, stack<int> &pilha);
        void dfs_transposto(vector<list<int>> &grafo, int vertice, vector<bool> &visitados);
        void kosaraju();
        vector<list<int>> grafo_transposto();

        int num_vertices;
        int capital;
        vector<int> distancias_ate_capital;
        vector<int> batalhoes;
        vector<vector<int>> area_batalhoes;
        vector<list<int>> lista_adj;
};

#endif
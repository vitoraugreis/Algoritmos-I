#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <list>
#include <stack>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class Grafo {
    public:
        Grafo(int num_vertices);
        ~Grafo();
        void adicionarAresta(int origem, int destino);
        void definirCapital();
        void definirBatalhoes();
        void definirRotas();

        int getCapital();
        vector<int> getBatalhoes();
        vector<int> getDistanciasAteCapital();
        vector<vector<int>> getRotas();
        void imprimirListaAdj();

    private:
        void bfs(int origem, int* distancia_total);
        void dfs_padrao(vector<list<int>> &grafo, int vertice, vector<bool> &visitados, stack<int> &pilha);
        void dfs_transposto(vector<list<int>> &grafo, int vertice, vector<bool> &visitados);
        void kosaraju();
        vector<list<int>> grafo_transposto();
        unordered_map<int, vector<int>> gerarSubgrafo(unordered_set<int> vertices);
        unordered_map<int, int> verticesBalanco(unordered_map<int, vector<int>> &grafo);
        vector<int> criarRota(unordered_map<int, vector<int>> &grafo, int batalao);
        void transformarEuleriano(unordered_map<int, vector<int>> &grafo, unordered_map<int, int> &balanco);
        int bfsRotas(unordered_map<int, vector<int>> &grafo, int origem, int destino);
        vector<pair<int,int>> hungaro(vector<vector<int>> &matriz);
        vector<int> bfsCaminho(unordered_map<int, vector<int>> &grafo, int origem, int destino);

        int num_vertices;
        vector<list<int>> lista_adj;

        int capital;
        vector<int> distancias_ate_capital;
        vector<int> batalhoes;
        vector<vector<int>> area_batalhoes;
        vector<vector<int>> rotas;
};

#endif
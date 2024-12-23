#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <queue>
#include <utility>

#define INF 10000000

using namespace std;

class Grafo {
    public:
        Grafo(int num_vertices);
        ~Grafo();
        void adicionarVertice(int id, int demanda, int index);
        void adicionarAresta(int comeco, int destino, int capacidade);
        int definirEnergiaTotal();
        int definirEnergiaNaoAtendida();
        int definirEnergiaPerdida();
        void imprimirConexoesCriticas();
    private:
        int num_vertices;
        int energia_total;
        int super_gerador_id;       // indice na matriz de adjacencia do super gerador.
        int super_sumidouro_id;     // indice na matriz de adjacencia do super sumidouro.
        vector<vector<int>> matriz_adj;
        vector<vector<int>> grafo_residual;
        unordered_map<int, pair<int, int>> vertices;
        vector<int> geradores;
        priority_queue<tuple<int, int, int>> arestas_saturadas;

        bool bfs(int origem, int destino, unordered_map<int, int> &caminho);
        int fordFulkerson(int origem, int destino);
        void identificarArestasSaturadas();

};

#endif
#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <queue>

#define INF 10000000

using namespace std;

class Vertice {
    public:
        Vertice(int id, int demanda);
        ~Vertice();
    private:
        int id;
        int demanda;
        void imprimirVertice();
    friend class Grafo;
};

class Aresta {
    public:
        Aresta(int destino, int capacidade);
        ~Aresta();
    private:
        int destino;
        int capacidade;
        void imprimirAresta();
    friend class Grafo;
};

class Grafo {
    public:
        Grafo(int num_vertices);
        ~Grafo();
        void adicionarVertice(int id, int demanda);
        void adicionarAresta(int comeco, int destino, int capacidade);
        int definirEnergiaTotal();
        int definirEnergiaNaoAtendida();
        int definirEnergiaPerdida();
        void imprimirGrafo();
    private:
        int num_vertices;
        int energia_total;
        unordered_map<int, int> geracao_por_gerador;
        unordered_map<int, Vertice> vertices;
        unordered_map<int, list<Aresta>> lista_adj;

        bool bfs(unordered_map<int, list<Aresta>> &grafo, int origem, int destino, unordered_map<int, int> &caminho);
        int fordFulkerson(unordered_map<int, list<Aresta>> &grafo_residual, int origem, int destino);

};

#endif
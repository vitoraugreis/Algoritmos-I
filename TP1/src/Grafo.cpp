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

int Grafo::definirCapital() {
    std::vector<std::pair<int,int>> candidatas;
    int qntd_candidatas = 0;

    for (int i = 0; i<this->num_vertices; i++) {
        int distancia_total = this->bfs(i);
        if (distancia_total) {
            candidatas.push_back(std::make_pair(distancia_total, i));
            qntd_candidatas++;
        }
    }

    int id_capital = 0;
    for (int i = 1; i<qntd_candidatas; i++) {
        if (candidatas[i].first < candidatas[id_capital].first) {
            id_capital = i;
        }
    }

    return candidatas[id_capital].second;
}

int Grafo::bfs(int origem) {
    std::queue<int> fila;
    std::vector<bool> visitados(this->num_vertices, false);
    std::vector<int> distancia(this->num_vertices, 0);
    int distancia_atual = 1;
    int distancia_total = 0;

    visitados[origem] = true;
    fila.push(origem);

    while (!fila.empty()) {
        int vertice_atual = fila.front();
        fila.pop();

        for (int i = 0; i<this->num_vertices; i++) {
            if (this->matriz_adj[vertice_atual][i] && !visitados[i]) {
                visitados[i] = true;
                distancia[i] = distancia_atual;
                fila.push(i);
            }
            distancia_atual++;
        }
    }
    
    for (int i = 0; i<this->num_vertices; i++) {
        if (visitados[i] == false) { return 0; }
        else { distancia_total += distancia[i]; }
    }

    return distancia_total;
}

void Grafo::imprimirMatrizAdj() {
    for (int i = 0; i<this->num_vertices; i++) {
        for (int j = 0; j<this->num_vertices; j++) {
            std::cout << this->matriz_adj[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}
#include "Grafo.hpp"

Grafo::Grafo(int num_vertices) 
    : num_vertices(num_vertices), lista_adj(num_vertices) {}

Grafo::~Grafo() {}

void Grafo::adicionarAresta(int origem, int destino) {
    lista_adj[origem].push_back(destino);
}

int Grafo::definirCapital() {
    vector<pair<int,int>> candidatas;
    int qntd_candidatas = 0;

    for (int i = 0; i<this->num_vertices; i++) {
        int distancia_total = this->bfs(i);
        if (distancia_total) {
            candidatas.push_back(make_pair(distancia_total, i));
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
    queue<int> fila;
    vector<bool> visitados(this->num_vertices, false);
    vector<int> distancia(this->num_vertices, 0);
    int distancia_atual = 1;
    int distancia_total = 0;

    visitados[origem] = true;
    fila.push(origem);

    while (!fila.empty()) {
        int vertice_atual = fila.front();
        fila.pop();

        for (int i : lista_adj[vertice_atual]) {
            if (!visitados[i]) {
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

void Grafo::dfs_padrao(vector<list<int>> &grafo, int vertice, vector<bool> &visitados, stack<int> &pilha) {
    visitados[vertice] = true;
    for (int i : grafo[vertice]) {
        if (!visitados[i]) { dfs_padrao(grafo, i, visitados, pilha); }
    }
    pilha.push(vertice);
}

void Grafo::dfs_transposto(vector<list<int>> &grafo, int vertice, vector<bool> &visitados) {
    visitados[vertice] = true;
    for (int i : grafo[vertice]) {
        if (!visitados[i]) { dfs_transposto(grafo, i, visitados); }
    }
}

vector<list<int>> Grafo::grafo_transposto() {
    vector<list<int>> lista_adj_T(num_vertices);
    for (int i = 0; i<num_vertices; i++) {
        for (int j : lista_adj[i]) { 
            lista_adj_T[j].push_back(i); 
        }
    }
    lista_adj = lista_adj_T;
    return lista_adj_T;
}

int Grafo::kosaraju() {
    stack<int> pilha;
    vector<bool> visitados(num_vertices, false);
    int cfc = 0;
    for (int i = 0; i<num_vertices; i++) {
        if (!visitados[i]) { dfs_padrao(lista_adj, i, visitados, pilha); }
    }

    vector<list<int>> transposto = grafo_transposto();
    for (int i = 0; i<num_vertices; i++) { visitados[i] = false; }

    while (!pilha.empty()) {
        int vertice = pilha.top();
        pilha.pop();

        if (!visitados[vertice]) {
            dfs_transposto(transposto, vertice, visitados);
            cfc++;
        }
    }
    return cfc;
}

void Grafo::imprimirListaAdj() {
    for (int i = 0; i<this->num_vertices; i++) {
        cout << i << " --> ";
        for (int j : lista_adj[i]) {
            cout << j << ' ';
        }
        cout << endl;
    }
}
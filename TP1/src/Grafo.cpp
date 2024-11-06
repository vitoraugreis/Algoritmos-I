#include "Grafo.hpp"

Grafo::Grafo(int num_vertices) 
    : num_vertices(num_vertices), lista_adj(num_vertices) {}

Grafo::~Grafo() {}

void Grafo::adicionarAresta(int origem, int destino) {
    lista_adj[origem].push_back(destino);
}

void Grafo::definirCapital() {
    int distancia_total = -1;
    distancias_ate_capital = vector<int>(num_vertices, 0);

    for (int i = 0; i<this->num_vertices; i++) {
        bfs(i, &distancia_total);
    }
}

void Grafo::bfs(int origem, int* distancia_total) {
    queue<pair<int, int>> fila;
    vector<bool> visitados(this->num_vertices, false);
    vector<int> distancia(this->num_vertices, 0);

    visitados[origem] = true;
    fila.push(make_pair(origem, 0));

    while (!fila.empty()) {
        int vertice_atual = fila.front().first;
        int distancia_atual = fila.front().second;
        fila.pop();

        for (int i : lista_adj[vertice_atual]) {
            if (!visitados[i]) {
                visitados[i] = true;
                distancia[i] = distancia_atual+1;
                fila.push(make_pair(i, distancia_atual+1));
            }
        }
        distancia_atual++;
    }
    
    int total = 0;
    for (int i = 0; i<this->num_vertices; i++) {
        if (visitados[i] == false) { return; }
        else { total += distancia[i]; }
    }

    if (total < *distancia_total || *distancia_total == -1) {
        *distancia_total = total;
        distancias_ate_capital = distancia;
        capital = origem;
    }
    return;
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

int Grafo::getCapital() {
    return capital;
}

vector<int> Grafo::getDistanciasAteCapital() {
    return distancias_ate_capital;
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
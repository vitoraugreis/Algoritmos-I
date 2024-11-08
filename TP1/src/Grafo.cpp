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

void Grafo::definirBatalhoes() {
    kosaraju();
    for (auto it = area_batalhoes.begin(); it != area_batalhoes.end(); it++) {
        int distancia_min = -1;
        int id_batalhao = -1;
        for (auto j = (*it).begin(); j != (*it).end(); j++) {
            if (distancia_min == -1) { 
                distancia_min = distancias_ate_capital[*j]; 
                id_batalhao = *j;
            } else {
                if (distancias_ate_capital[*j] < distancia_min) {
                    distancia_min = distancias_ate_capital[*j];
                    id_batalhao = *j;
                }
            }
        }
        if (distancia_min != 0) { batalhoes.push_back(id_batalhao); }
    }
}

void Grafo::definirRotas() {
    int index = 0;
    for (auto it = area_batalhoes.begin(); it != area_batalhoes.end(); it++) {
        unordered_set<int> vertices;
        int batalhao;
        for (auto jt = (*it).begin(); jt != (*it).end(); jt++) {
            vertices.insert(*jt);
        }

        if (vertices.find(capital) != vertices.end()) {
            if (vertices.size() > 1) {
                batalhao = capital;
            } else { continue; }
        } else  if  (vertices.size() > 1){
            batalhao = batalhoes[index];
            index++;
        } else { index++; continue; }

        // revisar estrutura d dados
        unordered_map<int, unordered_set<int>> subgrafo = gerarSubgrafo(vertices);
        for (auto jt = subgrafo.begin(); jt != subgrafo.end(); jt++) {
            if ((*jt).first == batalhao) { cout << "B"; }
            cout << (*jt).first << " --> ";

            for (auto kt = (*jt).second.begin(); kt != (*jt).second.end(); kt++) {
                cout << *kt << ' ';
            }
            cout << endl;
        }
        cout << "-----------------" << endl;
    }
    
}

unordered_map<int, unordered_set<int>> Grafo::gerarSubgrafo(unordered_set<int> vertices) {
    unordered_map<int, unordered_set<int>> subgrafo;
    for (int i = 0; i<num_vertices; i++) {
        if (vertices.find(i) == vertices.end()) { continue; }
        subgrafo[i] = unordered_set<int>();
        for (auto it = lista_adj[i].begin(); it != lista_adj[i].end(); it++) {
            if (vertices.find(*it) == vertices.end()) { continue; }
            subgrafo[i].insert(*it);
        }
    }

    return subgrafo;
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
    area_batalhoes[area_batalhoes.size()-1].push_back(vertice);
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

void Grafo::kosaraju() {
    stack<int> pilha;
    vector<bool> visitados(num_vertices, false);

    for (int i = 0; i<num_vertices; i++) {
        if (!visitados[i]) { dfs_padrao(lista_adj, i, visitados, pilha); }
    }

    vector<list<int>> transposto = grafo_transposto();
    for (int i = 0; i<num_vertices; i++) { visitados[i] = false; }

    while (!pilha.empty()) {
        int vertice = pilha.top();
        pilha.pop();

        if (!visitados[vertice]) {
            area_batalhoes.push_back(vector<int>());
            dfs_transposto(transposto, vertice, visitados);
        }
    }
    return;
}

int Grafo::getCapital() { return capital; }

vector<int> Grafo::getBatalhoes() { return batalhoes; }

vector<int> Grafo::getDistanciasAteCapital() { return distancias_ate_capital; }

void Grafo::imprimirListaAdj() {
    for (int i = 0; i<this->num_vertices; i++) {
        cout << i << " --> ";
        for (int j : lista_adj[i]) {
            cout << j << ' ';
        }
        cout << endl;
    }
}
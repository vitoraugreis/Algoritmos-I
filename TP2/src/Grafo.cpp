#include "Grafo.hpp"

Vertice::Vertice(int id, int demanda) {
    this->id = id;
    this->demanda = demanda;
}

Vertice::~Vertice() {}

void Vertice::imprimirVertice() {
    cout << id << "(" << demanda << ")";
}

Aresta::Aresta(int destino, int capacidade) {
    this->destino = destino;
    this->capacidade = capacidade;
}

Aresta::~Aresta() {}

void Aresta::imprimirAresta() {
    cout << destino << "(" << capacidade << ")";
}

Grafo::Grafo(int num_vertices) {
    this->num_vertices = num_vertices;
    vertices = unordered_map<int, Vertice>();
    lista_adj = unordered_map<int, list<Aresta>>();
    energia_total = 0;
}

Grafo::~Grafo() {}

void::Grafo::adicionarVertice(int id, int demanda) {
    vertices.insert({id, Vertice(id, demanda)});
    lista_adj.insert({id, list<Aresta>()});
}

void Grafo::adicionarAresta(int origem, int destino, int capacidade) {
    lista_adj[origem].push_back(Aresta(destino, capacidade));
}

void Grafo::imprimirGrafo() {
    for (auto it = lista_adj.begin(); it != lista_adj.end(); it++) {
        vertices.at(it->first).imprimirVertice();
        cout <<  " --> ";
        for (auto jt = it->second.begin(); jt != it->second.end(); jt++) {
            jt->imprimirAresta();
            cout << ' ';
        }
        cout << endl;
    }
}

int Grafo::definirEnergiaTotal() {
    for (auto it = lista_adj.begin(); it != lista_adj.end(); it++) {
        if (vertices.at(it->first).demanda == 0) { geracao_por_gerador[it->first] = 0; }
    }
    for (auto it = geracao_por_gerador.begin(); it != geracao_por_gerador.end(); it++) {
        unordered_map<int, list<Aresta>> grafo_residual = lista_adj;
        it->second = fordFulkerson(grafo_residual, it->first, -1);
    }

    if (geracao_por_gerador.size() == 1) { 
        energia_total = geracao_por_gerador.begin()->second;
        return energia_total;
    }

    unordered_map<int, list<Aresta>> grafo_residual = lista_adj;
    grafo_residual[0] = list<Aresta>();
    for (auto it = geracao_por_gerador.begin(); it != geracao_por_gerador.end(); it++) {
        grafo_residual[0].push_back(Aresta(it->first, it->second));
    }
    
    energia_total = fordFulkerson(grafo_residual, 0, -1);

    return energia_total;
}

int Grafo::definirEnergiaNaoAtendida() {
    int demanda_total = 0;
    for (auto it = vertices.begin(); it != vertices.end(); it++) {
        demanda_total += it->second.demanda;
    }
    
    return (demanda_total - energia_total) > 0 ? (demanda_total - energia_total) : 0;
}

int Grafo::definirEnergiaPerdida() {
    int energia_gerada = 0;
    for (auto it : geracao_por_gerador) {
        for (auto jt : lista_adj[it.first]) { energia_gerada += jt.capacidade; }
    }
    return energia_gerada - energia_total;
}

bool Grafo::bfs(unordered_map<int, list<Aresta>> &grafo, int origem, int destino, unordered_map<int, int> &caminho) {
    unordered_map<int, bool> visitado;
    for (auto it = grafo.begin(); it != grafo.end(); it++) { visitado[it->first] = false; }
    queue<int> fila;
    fila.push(origem);
    visitado[origem] = true;
    caminho[origem] = -1;

    while (!fila.empty()) {
        int atual = fila.front();
        fila.pop();

        for (auto it = grafo[atual].begin(); it != grafo[atual].end(); it++) {
            if (visitado[it->destino] == false && it->capacidade > 0) {
                if (it->destino == destino) {
                    caminho[it->destino] = atual;
                    return true;
                }
                fila.push(it->destino);
                caminho[it->destino] = atual;
                visitado[it->destino] = true;
            }
        }
    }
    return false;
}

int Grafo::fordFulkerson(unordered_map<int, list<Aresta>> &grafo_residual, int origem, int destino) {
    int u, v;
    for (auto it = grafo_residual.begin(); it != grafo_residual.end(); it++) {
        if (it->first == 0) { continue; }
        int demanda = vertices.at(it->first).demanda;
        if (demanda > 0) { it->second.push_back({Aresta(-1, demanda)}); }
    }
    int fluxo_max = 0;
    unordered_map<int, int> caminho;

    while (bfs(grafo_residual, origem, destino, caminho)) {
        int fluxo = INF;
        for (v = destino; v != origem; v = caminho[v]) {
            u = caminho[v];
            for (auto it = grafo_residual[u].begin(); it != grafo_residual[u].end(); it++) {
                if (it->destino == v) {
                    fluxo = min(it->capacidade, fluxo);
                    break;
                }
            }
        }

        for (v = destino; v != origem; v = caminho[v]) {
            u = caminho[v];
            for (auto it = grafo_residual[u].begin(); it != grafo_residual[u].end(); it++) {
                if (it->destino == v) {
                    it->capacidade -= fluxo;
                }
            }
            for (auto it = grafo_residual[v].begin(); it != grafo_residual[v].end(); it++) {
                if (it->destino == u) {
                    it->capacidade += fluxo;
                }
            }
        }
        fluxo_max += fluxo;
    }
    return fluxo_max;
}
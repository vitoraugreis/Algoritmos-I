#include "Grafo.hpp"

// Construtor.
Grafo::Grafo(int num_vertices) {
    this->num_vertices = num_vertices;
    energia_total = 0;
    matriz_adj = vector<vector<int>>(num_vertices, vector<int>(num_vertices, 0));
    grafo_residual = vector<vector<int>>(num_vertices+2, vector<int>(num_vertices+2, 0));
    unordered_map<int, pair<int, int>> vertices;
    super_gerador_id = num_vertices;
    super_sumidouro_id = num_vertices+1;
}

// Destrutor.
Grafo::~Grafo() {}

// Adiciona um vértice ao grafo com seu ID, demanda e índice na matriz de adjacência.
void Grafo::adicionarVertice(int id, int demanda, int index) {
    vertices[id] = make_pair(index, demanda);
    // Caso sua demanda seja 0, significa que é um gerador.
    // Se é um gerador, deve haver uma conexão entre ele e o super gerador.
    // Caso não seja gerador, deve haver uma conexão entre ele e o super sumidouro.
    if (demanda == 0) {
        grafo_residual[super_gerador_id][vertices[id].first] = INF;
        geradores.push_back(id);
    } else { grafo_residual[vertices[id].first][super_sumidouro_id] = demanda; }
}

// Adiciona uma aresta entre dois vértices no grafo original e no grafo residual.
void Grafo::adicionarAresta(int origem, int destino, int capacidade) {
    int origem_idx = vertices[origem].first;
    int destino_idx = vertices[destino].first;

    matriz_adj[origem_idx][destino_idx] = capacidade;
    grafo_residual[origem_idx][destino_idx] = capacidade;
}

// Define Energia Total.
int Grafo::definirEnergiaTotal() {
    energia_total = fordFulkerson(super_gerador_id, super_sumidouro_id);
    identificarArestasSaturadas();
    return energia_total;
}

// Define Energia não atendida
int Grafo::definirEnergiaNaoAtendida() {
    int demanda_total = 0;
    for (auto it = vertices.begin(); it != vertices.end(); it++) {
        demanda_total += it->second.second;
    }
    
    return (demanda_total - energia_total) > 0 ? (demanda_total - energia_total) : 0;
}

// Define Energia Perdida.
int Grafo::definirEnergiaPerdida() {
    int energia_gerada = 0;

    for (auto it = geradores.begin(); it != geradores.end(); it++) {
        int gerador_idx = vertices[*it].first;
        for (int capacidade : matriz_adj[gerador_idx]) {
            energia_gerada += capacidade;
        }
    }
    return energia_gerada - energia_total;
}

// Impressão de conexões críticas, em ordem decrescente.
void Grafo::imprimirConexoesCriticas() {
    cout << arestas_saturadas.size() << endl;

    while (!arestas_saturadas.empty()) {
        auto aresta = arestas_saturadas.top();
        cout << get<1>(aresta) << ' ' << get<2>(aresta) << ' ' << get<0>(aresta) << endl;
        arestas_saturadas.pop();
    }
}

// Busca em largura para o algoritmo de Ford-Fulkerson.
bool Grafo::bfs(int origem, int destino, unordered_map<int, int>& caminho) {
    vector<bool> visitado(num_vertices+2, false);
    queue<int> fila;
    fila.push(origem);
    visitado[origem] = true;
    caminho[origem] = -1;

    while (!fila.empty()) {
        int atual = fila.front();
        fila.pop();

        for (int v = 0; v < num_vertices+2; v++) {
            if (!visitado[v] && grafo_residual[atual][v] > 0) {
                if (v == destino) {
                    caminho[v] = atual;
                    return true;
                }
                fila.push(v);
                caminho[v] = atual;
                visitado[v] = true;
            }
        }
    }
    return false;
}

int Grafo::fordFulkerson(int origem, int destino) {
    int u, v;
    int fluxo_max = 0;
    unordered_map<int, int> caminho;

    // Verifica se há caminho aumentante restante no grafo.
    while (bfs(origem, destino, caminho)) {
        int fluxo = INF;
        // Calculo o fluxo do caminho atual.
        for (v = destino; v != origem; v = caminho[v]) {
            u = caminho[v];
            fluxo = min(fluxo, grafo_residual[u][v]);
        }
        // Reduz da capacidade de cada aresta do caminho o fluxo calculado.
        for (v = destino; v != origem; v = caminho[v]) {
            u = caminho[v];
            grafo_residual[u][v] -= fluxo;
            grafo_residual[v][u] += fluxo;
        }
        fluxo_max += fluxo;
    }

    return fluxo_max;
}

// Identifica as arestas saturadas.
void Grafo::identificarArestasSaturadas() {
    for (auto it = vertices.begin(); it != vertices.end(); it++) {
        for (auto jt = vertices.begin(); jt != vertices.end(); jt++) {
            int origem = it->second.first;
            int destino = jt->second.first;
            if (grafo_residual[origem][destino] == 0) {
                if (matriz_adj[origem][destino] > 0) {
                    arestas_saturadas.push(make_tuple(
                        matriz_adj[origem][destino], it->first, jt->first));
                }
            }
        }
    }
}
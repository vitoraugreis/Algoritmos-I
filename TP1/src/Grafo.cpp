#include "Grafo.hpp"

Grafo::Grafo(int num_vertices) 
    : num_vertices(num_vertices), lista_adj(num_vertices) {}

Grafo::~Grafo() {}

// Adiciona Aresta no grafo.
void Grafo::adicionarAresta(int origem, int destino) {
    lista_adj[origem].push_back(destino);
}

// CAPITAL: Função principal - Definição da capital.
void Grafo::definirCapital() {
    int distancia_total = -1;
    distancias_ate_capital = vector<int>(num_vertices, 0);

    // Faz uma busca em largura em todos os vértices do grafo.
    // O que tiver conexão com todos os demais vértices e uma distância total menor é definido como capital.
    for (int i = 0; i<this->num_vertices; i++) {
        bfsCapital(i, &distancia_total);
    }
}

// CAPITAL: Busca em largura para definição da capital do império.
void Grafo::bfsCapital(int origem, int* distancia_total) {
    queue<pair<int, int>> fila; // Fila que armazena o índice do vértice visitado e a distância da capital até ele.
    vector<bool> visitados(this->num_vertices, false);
    vector<int> distancia(this->num_vertices, 0);

    visitados[origem] = true;
    fila.push(make_pair(origem, 0));

    // Busca em largura.
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

    // Calcula a distância total.
    for (int i = 0; i<this->num_vertices; i++) {
        if (visitados[i] == false) { return; }
        else { total += distancia[i]; }
    }

    // Se a distância total encontrada for menor que a menor encontrada anteriormente, altera as informações da capital.
    if (total < *distancia_total || *distancia_total == -1) {
        *distancia_total = total;
        distancias_ate_capital = distancia;
        capital = origem;
    }
    return;
}

// BATALHÕES: Função principal - Definição dos batalhões.
void Grafo::definirBatalhoes() {
    // Utiliza o Algoritmo de Kosaraju para obter todas as componentes fortemente conectadas (CFC) do grafo.
    // Cada CFC terá um batalhão.
    kosaraju();

    // A partir das CFC's definidas, verifica qual vértice tem a menor distância até a capital e o define como batalhão.
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

// BATALHÕES: Algoritmo de Kosaraju.
void Grafo::kosaraju() {
    stack<int> pilha;
    vector<bool> visitados(num_vertices, false);

    // Usa uma busca em profundidade (DFS) no grafo e computa a ordem de término em uma pilha.
    for (int i = 0; i<num_vertices; i++) {
        if (!visitados[i]) { dfs_padrao(lista_adj, i, visitados, pilha); }
    }

    // Gera uma nova lista de adjacência, mas com as arestas com sentidos invertidos.
    vector<list<int>> transposto = grafo_transposto();
    for (int i = 0; i<num_vertices; i++) { visitados[i] = false; }

    // Usa uma DFS novamente, mas no grafo transposto e com início no primeiro vértice não visitado da pilha.
    // Como resultado, temos os vértices, em um vetor, que compõem cada componente conexa, que é inserido na área dos batalhões.
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

// BATALHÕES: Busca em profundidade no grafo original.
void Grafo::dfs_padrao(vector<list<int>> &grafo, int vertice, vector<bool> &visitados, stack<int> &pilha) {
    visitados[vertice] = true;
    for (int i : grafo[vertice]) {
        if (!visitados[i]) { dfs_padrao(grafo, i, visitados, pilha); }
    }
    pilha.push(vertice);
}

// BATALHÕES: Geração do grafo transposto ( Inversão no sentido das arestas )
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

// BATALHÕES: Busca em profundidade no grafo transposto.
void Grafo::dfs_transposto(vector<list<int>> &grafo, int vertice, vector<bool> &visitados) {
    visitados[vertice] = true;
    area_batalhoes[area_batalhoes.size()-1].push_back(vertice);
    for (int i : grafo[vertice]) {
        if (!visitados[i]) { dfs_transposto(grafo, i, visitados); }
    }
}

// ROTAS: Função principal - Definição das rotas.
void Grafo::definirRotas() {
    rotas = vector<vector<int>>();
    int index = 0;  // Usado na obtenção do batalhão da CFC analisada.

    // Para cada CFC gerada, há a tentativa de geração de uma rota.
    for (auto it = area_batalhoes.begin(); it != area_batalhoes.end(); it++) {
        unordered_set<int> vertices;
        int batalhao;

        // Insere todos os vertices da atual CFC no set.
        for (auto jt = (*it).begin(); jt != (*it).end(); jt++) {
            vertices.insert(*jt);
        }

        // Obtém o batalhão da CFC. Caso o tamanho da CFC seja igual a 1, não há rota a ser gerada.
        if (vertices.find(capital) != vertices.end()) {
            if (vertices.size() > 1) {
                batalhao = capital;
            } else { continue; }
        } else if (vertices.size() > 1){
            batalhao = batalhoes[index];
            index++;
        } else { index++; continue; }

        // Gera um subgrafo que contém apenas as arestas que compôe a CFC.
        unordered_map<int, vector<int>> subgrafo = gerarSubgrafo(vertices);

        // Verifica o balanço de cada vértice da CFC.
        unordered_map<int, int> balanco = verticesBalanco(subgrafo);

        bool eh_euleriano = true;

        // Caso encontre algum vértice desbalanceado, significa que o grafo não é euleriano.
        for (auto it = balanco.begin(); it != balanco.end(); it++) {
            if ((*it).second != 0) { eh_euleriano = false; break; }
        }

        // Caso o grafo seja euleriano, basta criar a rota.
        if (eh_euleriano) {
            vector<int> rota_atual = criarRota(subgrafo, batalhao);
            rotas.push_back(rota_atual);
            continue;
        }

        // Se o grafo não é euleriano, vamos transformar ele em um euleriano e gerar a rota.
        transformarEuleriano(subgrafo, balanco);
        vector<int> rota_atual = criarRota(subgrafo, batalhao);
        rotas.push_back(rota_atual);

    }
}

// ROTAS: Gera o subgrafo, que se refere a área em que o batalhão cobre.
unordered_map<int, vector<int>> Grafo::gerarSubgrafo(unordered_set<int> vertices) {
    unordered_map<int, vector<int>> subgrafo;
    for (int i = 0; i<num_vertices; i++) {
        if (vertices.find(i) == vertices.end()) { continue; }
        subgrafo[i] = vector<int>();

        // Insere todas as arestas do vértice que compôe a CFC na lista de adjacência.
        for (auto it = lista_adj[i].begin(); it != lista_adj[i].end(); it++) {
            if (vertices.find(*it) == vertices.end()) { continue; }
            subgrafo[i].push_back(*it);
        }
    }

    return subgrafo;
}

// ROTAS: Verifica o balanceamento de todos os vértices de um grafo.
unordered_map<int, int> Grafo::verticesBalanco(unordered_map<int, vector<int>> &grafo) {
    unordered_map<int, int> balanco;
    for (auto it = grafo.begin(); it != grafo.end(); it++) {
        balanco.insert(make_pair((*it).first, 0));
    }

    // O balanço do vértice é o resultado da seguinte operação:
    // n° de arestas que saem do vértice - n° de arestas que entram no vértice.
    // Caso o balanço seja igual a 0, significa que o vértice é balanceado.
    for (auto it = grafo.begin(); it != grafo.end(); it++) {
        for (auto jt = (*it).second.begin(); jt != (*it).second.end(); jt++) {
            balanco[(*it).first]++;
            balanco[*jt]--;
        }
    }

    return balanco;
}

// ROTAS: Transforma um grafo não euleriano em um euleriano.
void Grafo::transformarEuleriano(unordered_map<int, vector<int>> &grafo, unordered_map<int, int> &balanco) {
    vector<int> positivos;
    vector<int> negativos;

    // Preenche os vetores de balanços negativos e positivos de acordo com o balanço do vértice.
    // Um mesmo vértice pode aparecer duas vezes em um vetor, caso o seu balançõ não seja -1 ou 1.
    // Isso irá garantir que ambos os vetores tenham a mesma dimensão.
    for (auto it = balanco.begin(); it != balanco.end(); it++) {
        if ((*it).second > 0) {
            while ((*it).second != 0) {
                positivos.push_back((*it).first);
                (*it).second--;
            }
        } else if ((*it).second < 0) {
            while ((*it).second != 0) {
                negativos.push_back((*it).first);
                (*it).second++;
            }
        }
    }

    // Cria uma matriz de distâncias entre o vértice de balanço negativo [i] até o vértice de balanço positivo [j].
    // Para descobrir a distância entre os vértices, é usado uma busca em largura.
    vector<vector<int>> distancias;
    for (int i = 0; i<int(positivos.size()); i++) {
        distancias.push_back(vector<int>());
        for (int j = 0; j<int(positivos.size()); j++) {
            distancias[i].push_back(bfsRotas(grafo, negativos[i], positivos[j]));
        }
    }

    // Usa o algoritmo húngaro para fazer o emparelhamento perfeito na matriz de distâncias entre os vértices.
    // Irá retornar qual caminho devemos percorrer e duplicar as arestas.
    // Isso garante o menor caminho da rota.
    vector<pair<int, int>> hung = hungaro(distancias);

    // Gera o caminho entre cada emparelhamento de vértices e duplica as arestas que são percorridas.
    // Usada uma BFS que retorna o caminho percorrido.
    // Dessa forma o grafo é totalmente balanceado e considerado euleriano.
    for (auto it = hung.begin(); it != hung.end(); it++) {
        vector<int> caminho = bfsCaminho(grafo, negativos[(*it).first], positivos[(*it).second]);
        for (int i = 1; i<int(caminho.size()); i++) {
            grafo[caminho[i-1]].push_back(caminho[i]);
        }
    }

    return;
}

// ROTAS: Busca em Largura para obter a distância entre o vértice de balanço negativo até o vértice de balanço positivo.
int Grafo::bfsRotas(unordered_map<int, vector<int>> &grafo, int origem, int destino) {
    queue<pair<int, int>> fila;
    vector<bool> visitados(grafo.size(), false);

    visitados[origem] = true;
    fila.push(make_pair(origem, 0));

    while (!fila.empty()) {
        int vertice_atual = fila.front().first;
        int distancia_atual = fila.front().second;
        fila.pop();

        for (int i : grafo[vertice_atual]) {
            if (!visitados[i]) {
                if (i == destino) { return distancia_atual + 1; }
                visitados[i] = true;
                fila.push(make_pair(i, distancia_atual+1));
            }
        }
    }
    return -1;
}

// ROTAS: Algoritmo Hungaro para fazer o emparelhamento dos vértices em relação as suas distâncias.
vector<pair<int, int>> Grafo::hungaro(vector<vector<int>> &matriz) {
    int dim = int(matriz.size());

    // Redução de linhas.
    for (int i = 0; i<dim; i++) {
        int min = matriz[i][0];
        for (int j = 1; j<dim; j++) {
            if (matriz[i][j] < min) { min = matriz[i][j]; }
        }
        for (int j = 0; j<dim; j++) { matriz[i][j] -= min; }
    }

    // Redução de colunas.
    for (int j = 0; j<dim; j++) {
        int min = matriz[0][j];
        for (int i = 1; i<dim; i++) {
            if (matriz[i][j] < min) { min = matriz[i][j]; }
        }
        for (int i = 0; i<dim; i++) { matriz[i][j] -= min; }
    }

    // Cobertura de zeros.
    vector<bool> linhas_cobertas(dim, false);
    vector<bool> colunas_cobertas(dim, false);

    // Marcação das linhas e colunas que contém zero.
    for (int i = 0; i<dim; i++) {
        for (int j = 0; j<dim; j++) {
            if (matriz[i][j] == 0 && !linhas_cobertas[i] && !colunas_cobertas[j]) {
                linhas_cobertas[i] = true;
                colunas_cobertas[j] = true;
                break;
            }
        }
    }

    // Estende a cobertura para linhas e colunas.
    bool concluido = false;
    while (!concluido) {
        concluido = true;
        for (int i = 0; i<dim; i++) {
            if (linhas_cobertas[i]) {
                for (int j = 0; j<dim; j++) {
                    if (matriz[i][j] == 0 && !colunas_cobertas[j]) {
                        colunas_cobertas[j] = true;
                        concluido = false;
                    }
                }
            }
        }

        for (int j = 0; j<dim; j++) {
            if (colunas_cobertas[j]) {
                for (int i = 0; i<dim; i++) {
                    if (matriz[i][j] == 0 && !linhas_cobertas[i]) {
                        linhas_cobertas[i] = true;
                        concluido = false;
                    }
                }
            }
        }
    }

    int min = 10000000;
    bool foi_descoberto = false;

    // Encontra o menor valor não coberto.
    for (int i = 0; i<dim; i++) {
        for (int j = 0; j<dim; j++) {
            if (!linhas_cobertas[i] && !colunas_cobertas[j] && matriz[i][j] < min) {
                min = matriz[i][j];
                foi_descoberto = true;
            }
        }
    }

    // Ajusta a matriz.
    if (foi_descoberto) {
        for (int i = 0; i<dim; i++) {
            for (int j = 0; j<dim; j++) {
                if (!linhas_cobertas[i] && !colunas_cobertas[j]) { matriz[i][j] -= min; }
                if (linhas_cobertas[i] && colunas_cobertas[j]) { matriz[i][j] += min; }
            }
        }
    }

    vector<pair<int, int>> emparelhamento;
    vector<bool> linhas_emparelhada(dim, false), colunas_emparelhadas(dim, false);

    // Atribui cada par do emparelhamento e os retorna.
    for (int i = 0; i<dim; i++) {
        for (int j = 0; j<dim; j++) {
            if (matriz[i][j] == 0 && !linhas_emparelhada[i] && !linhas_emparelhada[j]) {
                emparelhamento.push_back(make_pair(i, j));
                linhas_emparelhada[i] = true;
                colunas_emparelhadas[j] = true;
                break;
            }
        }
    }

    return emparelhamento;
}

// ROTAS: Busca em largura que retorna um vetor com o caminho percorrido.
vector<int> Grafo::bfsCaminho(unordered_map<int, vector<int>> &grafo, int origem, int destino) {
    vector<bool> visitados(grafo.size(), false);
    unordered_map<int, int> antecessor; // Armazena o vértice antecessor a outro.
    queue<int> fila;

    fila.push(origem);
    visitados[origem] = true;
    antecessor[origem] = -1;

    while (!fila.empty()) {
        int atual = fila.front();
        fila.pop();

        // Se vértice de destino alcançado, percorre o caminho de trás para frente e depois inverte o vetor e o retorna.
        if (atual == destino) {
            vector<int> caminho_inv, caminho;
            for (int v = destino; v != -1; v = antecessor[v]) {
                caminho_inv.push_back(v);
            }
            for (auto it = caminho_inv.rbegin(); it != caminho_inv.rend(); it++) {
                caminho.push_back(*it);
            }
            return caminho;
        }

        for (int vizinho : grafo[atual]) {
            if (!visitados[vizinho]) {
                fila.push(vizinho);
                visitados[vizinho] = true;
                antecessor[vizinho] = atual;
            }
        }
    }
    return {};
}

// ROTAS: Cria a rota a partir de um grafo euleriano. Baseado no algoritmo de Hierholzer.
vector<int> Grafo::criarRota(unordered_map<int, vector<int>> &grafo, int batalao) {
    unordered_map<int, int> qntd_aresta;
    stack<int> caminho_atual;
    vector<int> rota;
    for (auto it = grafo.begin(); it != grafo.end(); it++) {
        qntd_aresta[(*it).first] = int((*it).second.size());
    }

    // Inicia o caminho no batalhão.
    caminho_atual.push(batalao);
    int vertice_atual = batalao;

    while (!caminho_atual.empty()) {
        // Se ainda há arestas a serem percorridas pelo vértice, o adiciona no caminho e vai para o próximo vértice.
        if (qntd_aresta[vertice_atual]) {
            caminho_atual.push(vertice_atual);
            int proximo_vertice = grafo[vertice_atual].back();
            qntd_aresta[vertice_atual]--;
            grafo[vertice_atual].pop_back();
            vertice_atual = proximo_vertice;
        } else {
            rota.push_back(vertice_atual);
            vertice_atual = caminho_atual.top();
            caminho_atual.pop();
        }
    }
    rota.pop_back();
    return rota;
}

// Funções gets.
int Grafo::getCapital() { return capital; }

vector<int> Grafo::getBatalhoes() { return batalhoes; }

vector<int> Grafo::getDistanciasAteCapital() { return distancias_ate_capital; }

vector<vector<int>> Grafo::getRotas() { return rotas; }

// Imprime a lista de adjacência ( Para fins de verificação )
void Grafo::imprimirListaAdj() {
    for (int i = 0; i<this->num_vertices; i++) {
        cout << i << " --> ";
        for (int j : lista_adj[i]) {
            cout << j << ' ';
        }
        cout << endl;
    }
}
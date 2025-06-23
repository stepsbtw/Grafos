#include "grafo.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <algorithm>
#include <iomanip>

Grafo::Grafo(int V, Representacao rep) : V(V), E(0), rep(rep) {
    if (rep == LISTA_ADJ) {
        lista_adj.resize(V);
    } else {
        matriz_adj.assign(V, vector<double>(V, 0.0));
    }
}

void Grafo::ler_arquivo(const string& nome_arquivo) {
    ifstream in(nome_arquivo);
    if (!in) {
        cerr << "Erro ao abrir arquivo." << endl;
        return;
    }

    in >> V;
    E = 0;
    if (rep == LISTA_ADJ) lista_adj.assign(V, vector<pair<int, double>>());
    else matriz_adj.assign(V, vector<double>(V, 0.0));

    tem_pesos = false;
    tem_pesos_negativos = false;

    int u, v;
    double peso;
    while (in >> u >> v >> peso) {
        if(peso != 0){
            tem_pesos = true;
        }
        if(peso < 0){
            tem_pesos_negativos = true;
        }
        if (rep == LISTA_ADJ) {
            lista_adj[u].push_back(make_pair(v, peso));
            lista_adj[v].push_back(make_pair(u, peso));
        } else {
            matriz_adj[u][v] = peso;
            matriz_adj[v][u] = peso;
        }
        E++;
    }
    E /= 2; // não-direcionado
}

void Grafo::salvar_estatisticas(const string& nome_arquivo) {
    vector<int> graus(V, 0);

    if (rep == LISTA_ADJ) {
        for (int i = 0; i < V; i++) {
            graus[i] = lista_adj[i].size();
        }
    } else {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (matriz_adj[i][j] > 0.0)
                    graus[i]++;
            }
        }
    }

    double grau_medio = 0.0;
    for (int g : graus) grau_medio += g;
    grau_medio /= V;

    vector<int> contagem(V + 1, 0);
    for (int g : graus) contagem[g]++;

    ofstream out(nome_arquivo);
    out << "# m = " << V << "\n";
    out << "# n = " << E << "\n";
    out << fixed << setprecision(1) << "# d_medio = " << grau_medio << "\n";

    for (int i = 0; i <= V; i++) {
        if (contagem[i] > 0) {
            out << i << " " << fixed << setprecision(2) << (double)contagem[i] / V << "\n";
        }
    }
}

void Grafo::bfs_interno(int s, vector<int>& pai, vector<int>& nivel) {
    vector<bool> visitado(V, false);
    queue<int> fila;

    visitado[s] = true;
    nivel[s] = 0;
    pai[s] = -1;
    fila.push(s);

    while (!fila.empty()) {
        int u = fila.front();
        fila.pop();

        if (rep == LISTA_ADJ) {
            for (size_t i = 0; i < lista_adj[u].size(); ++i) {
                int v = lista_adj[u][i].first;
                if (!visitado[v]) {
                    visitado[v] = true;
                    pai[v] = u;
                    nivel[v] = nivel[u] + 1;
                    fila.push(v);
                }
            }
        } else {
            for (int v = 0; v < V; v++) {
                if (matriz_adj[u][v] > 0.0 && !visitado[v]) {
                    visitado[v] = true;
                    pai[v] = u;
                    nivel[v] = nivel[u] + 1;
                    fila.push(v);
                }
            }
        }
    }
}

void Grafo::bfs_interno(int s, int t, vector<int>& pai, vector<int>& nivel) {
    vector<bool> visitado(V, false);
    queue<int> fila;

    visitado[s] = true;
    nivel[s] = 0;
    pai[s] = -1;
    fila.push(s);

    while (!fila.empty()) {
        int u = fila.front();
        fila.pop();

        if (rep == LISTA_ADJ) {
            for (size_t i = 0; i < lista_adj[u].size(); ++i) {
                int v = lista_adj[u][i].first;
                if (!visitado[v]) {
                    visitado[v] = true;
                    pai[v] = u;
                    nivel[v] = nivel[u] + 1;
                    fila.push(v);
                }
                if (v == t){
                    return;
                }
            }
        } else {
            for (int v = 0; v < V; v++) {
                if (matriz_adj[u][v] > 0.0 && !visitado[v]) {
                    visitado[v] = true;
                    pai[v] = u;
                    nivel[v] = nivel[u] + 1;
                    fila.push(v);
                }
                if (v == t){
                    return;
                }
            }
        }
    }
}

void Grafo::bfs(int s, const string& nome_saida) {
    vector<int> pai(V, -1), nivel(V, -1);
    bfs_interno(s, pai, nivel);

    ofstream out(nome_saida);
    out << "# BFS a partir do vértice " << s << "\n";
    out << "# Vertice | Pai | Nível\n";
    for (int i = 0; i < V; i++) {
        out << i << " " << pai[i] << " " << nivel[i] << "\n";
    }
}

void Grafo::dfs_interno(int v, vector<bool>& visitado, vector<int>& pai, vector<int>& pre, vector<int>& post, int& tempo) {
    visitado[v] = true;
    pre[v] = tempo++;

    if (rep == LISTA_ADJ) {
        for (size_t i = 0; i < lista_adj[v].size(); ++i) {
            int u = lista_adj[v][i].first;
            if (!visitado[u]) {
                pai[u] = v;
                dfs_interno(u, visitado, pai, pre, post, tempo);
            }
        }
    } else {
        for (int u = 0; u < V; u++) {
            if (matriz_adj[v][u] > 0.0 && !visitado[u]) {
                pai[u] = v;
                dfs_interno(u, visitado, pai, pre, post, tempo);
            }
        }
    }

    post[v] = tempo++;
}

void Grafo::dfs(int s, const string& nome_saida) {
    vector<int> pai(V, -1), pre(V, -1), post(V, -1);
    vector<bool> visitado(V, false);
    int tempo = 0;
    dfs_interno(s, visitado, pai, pre, post, tempo);

    ofstream out(nome_saida);
    out << "# DFS a partir do vértice " << s << "\n";
    out << "# Vertice Pai\n";
    for (int i = 0; i < V; i++) {
        out << i << " " << pai[i] << "\n";
    }
}

void Grafo::dfs_componentes(int v, vector<bool>& visitado, vector<int>& componente) {
    visitado[v] = true;
    componente.push_back(v);

    if (rep == LISTA_ADJ) {
        for (size_t i = 0; i < lista_adj[v].size(); ++i) {
            int u = lista_adj[v][i].first;
            if (!visitado[u]) {
                dfs_componentes(u, visitado, componente);
            }
        }
    } else {
        for (int u = 0; u < V; u++) {
            if (matriz_adj[v][u] > 0.0 && !visitado[u]) {
                dfs_componentes(u, visitado, componente);
            }
        }
    }
}

void Grafo::componentes_conexas(const string& nome_saida) {
    vector<bool> visitado(V, false);
    vector<vector<int>> componentes;

    for (int i = 0; i < V; i++) {
        if (!visitado[i]) {
            vector<int> componente;
            dfs_componentes(i, visitado, componente);
            componentes.push_back(componente);
        }
    }

    sort(componentes.begin(), componentes.end(), [](const vector<int>& a, const vector<int>& b) {
        return a.size() > b.size();
    });

    ofstream out(nome_saida);
    out << "# Numero de componentes: " << componentes.size() << "\n";
    for (size_t i = 0; i < componentes.size(); i++) {
        out << "Componente " << i + 1 << " (tam: " << componentes[i].size() << "): ";
        for (int v : componentes[i]) {
            out << v << " ";
        }
        out << "\n";
    }
}

#include <queue>
#include <limits>
#include <utility> // para std::pair

// Função auxiliar: cria uma fila de prioridade min-heap
struct Comparador {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        return a.first > b.first; // menor distância tem prioridade
    }
};

// Dijkstra para todos os vértices a partir de s
void Grafo::dijkstra(int s, vector<int>& pai, vector<int>& dists) {
    dists.assign(V, numeric_limits<int>::max());
    pai.assign(V, -1);
    dists[s] = 0;

    priority_queue< pair<int, int>, vector< pair<int, int> >, Comparador > fila;
    fila.push(make_pair(0, s));

    while (!fila.empty()) {
        pair<int, int> topo = fila.top();
        int dist_u = topo.first;
        int u = topo.second;
        fila.pop();

        if (dist_u > dists[u]) continue;

        if (rep == LISTA_ADJ) {
            for (size_t i = 0; i < lista_adj[u].size(); ++i) {
                int v = lista_adj[u][i].first;
                double peso = lista_adj[u][i].second;

                if (dists[u] + peso < dists[v]) {
                    dists[v] = dists[u] + peso;
                    pai[v] = u;
                    fila.push(make_pair(dists[v], v));
                }
            }
        } else if (rep == MATRIZ_ADJ) {
            for (int v = 0; v < V; ++v) {
                double peso = matriz_adj[u][v];
                if (peso > 0) { // Considera que peso 0 significa ausência de aresta
                    if (dists[u] + peso < dists[v]) {
                        dists[v] = dists[u] + peso;
                        pai[v] = u;
                        fila.push(make_pair(dists[v], v));
                    }
                }
            }
        }
    }
}

// Dijkstra do vértice s até t
void Grafo::dijkstra(int s, int t, vector<int>& pai, vector<int>& dists) {
    dists.assign(V, numeric_limits<int>::max());
    pai.assign(V, -1);
    dists[s] = 0;

    priority_queue< pair<int, int>, vector< pair<int, int> >, Comparador > fila;
    fila.push(make_pair(0, s));

    while (!fila.empty()) {
        pair<int, int> topo = fila.top();
        int dist_u = topo.first;
        int u = topo.second;
        fila.pop();

        if (u == t) break;
        if (dist_u > dists[u]) continue;

        if (rep == LISTA_ADJ) {
            for (size_t i = 0; i < lista_adj[u].size(); ++i) {
                int v = lista_adj[u][i].first;
                double peso = lista_adj[u][i].second;

                if (dists[u] + peso < dists[v]) {
                    dists[v] = dists[u] + peso;
                    pai[v] = u;
                    fila.push(make_pair(dists[v], v));
                }
            }
        } else if (rep == MATRIZ_ADJ) {
            for (int v = 0; v < V; ++v) {
                double peso = matriz_adj[u][v];
                if (peso > 0) {
                    if (dists[u] + peso < dists[v]) {
                        dists[v] = dists[u] + peso;
                        pai[v] = u;
                        fila.push(make_pair(dists[v], v));
                    }
                }
            }
        }
    }
}


pair<vector<int>,vector<int>> Grafo::caminho_minimo(int s, int t, const string& nome_arquivo){
    vector<int> pai(V, -1);
    vector<int> dists(V, -1);
    if(!tem_pesos){    
        bfs_interno(s, t, pai, dists);
    }
    else{
        if(!tem_pesos_negativos){
            dijkstra(s, t, pai, dists);
        }
        else{
            printf("Bellman Ford!");
        }
    }

    vector<int> caminho;
    int atual = t;
    while (atual != -1) {
        caminho.push_back(atual);
        atual = pai[atual];
    }

    reverse(caminho.begin(), caminho.end());

    ofstream fout(nome_arquivo.c_str());
    fout << "Caminho minimo de " << s << " ate " << t << ":" << endl;
    for (size_t i = 0; i < caminho.size(); ++i) {
        fout << caminho[i];
        if (i + 1 < caminho.size()) fout << " -> ";
    }
    fout << "\nDistancia total: " << dists[t] << endl;
    fout.close();

    return make_pair(caminho, dists);
}

pair<vector<vector<int>>, vector<int>> Grafo::caminhos_minimos(int s, const string& nome_arquivo) {
    vector<int> pai(V, -1);
    vector<int> dists(V, -1);

    if (!tem_pesos) {
        bfs_interno(s, pai, dists);
    } else {
        if (!tem_pesos_negativos) {
            dijkstra(s, pai, dists);
        } else {
            printf("Bellman Ford ainda nao implementado!\n");
            // Retorna vazio para não travar compilação
            return make_pair(vector<vector<int>>(), vector<int>());
        }
    }

    // Reconstrói o caminho mínimo para cada vértice
    vector<vector<int>> caminhos(V);

    for (int v = 0; v < V; ++v) {
        if (dists[v] == -1 || dists[v] == numeric_limits<int>::max()) {
            // Sem caminho
            caminhos[v] = vector<int>(); // vazio
        } else {
            vector<int> caminho;
            int atual = v;
            while (atual != -1) {
                caminho.push_back(atual);
                atual = pai[atual];
            }
            std::reverse(caminho.begin(), caminho.end());
            caminhos[v] = caminho;
        }
    }

    ofstream fout(nome_arquivo.c_str());
    if (!fout.is_open()) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo.c_str());
        return make_pair(caminhos, dists);
    }

    for (int v = 0; v < V; ++v) {
        fout << "Destino: " << v << "\n";
        if (caminhos[v].empty()) {
            fout << "  Nao existe caminho de " << s << " ate " << v << ".\n\n";
        } else {
            fout << "  Distancia: " << dists[v] << "\n";
            fout << "  Caminho: ";
            for (size_t i = 0; i < caminhos[v].size(); ++i) {
                fout << caminhos[v][i];
                if (i + 1 < caminhos[v].size())
                    fout << " -> ";
            }
            fout << "\n\n";
        }
    }

    fout.close();
    return make_pair(caminhos, dists);
}

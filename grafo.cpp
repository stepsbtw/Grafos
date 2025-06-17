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
        matriz_adj.assign(V, vector<int>(V, 0));
    }
}

void Grafo::ler_arquivo(const string& nome_arquivo) {
    ifstream in(nome_arquivo);
    if (!in) {
        cerr << "Erro ao abrir arquivo." << endl;
        return;
    }

    in >> V;
    if (rep == LISTA_ADJ) lista_adj.assign(V, vector<int>());
    else matriz_adj.assign(V, vector<int>(V, 0));

    int u, v;
    E = 0;
    while (in >> u >> v) {
        if (rep == LISTA_ADJ) {
            lista_adj[u].push_back(v);
            lista_adj[v].push_back(u);
        } else {
            matriz_adj[u][v] = 1;
            matriz_adj[v][u] = 1;
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
                graus[i] += matriz_adj[i][j];
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
    queue<int> q;
    visitado[s] = true;
    nivel[s] = 0;
    pai[s] = -1;
    q.push(s);

    while (!q.empty()) {
        int u = q.front(); q.pop();

        const vector<int>& vizinhos = (rep == LISTA_ADJ) ? lista_adj[u] : vector<int>();
        for (int v = 0; v < V; v++) {
            if ((rep == MATRIZ_ADJ && matriz_adj[u][v]) || (rep == LISTA_ADJ && find(vizinhos.begin(), vizinhos.end(), v) != vizinhos.end())) {
                if (!visitado[v]) {
                    visitado[v] = true;
                    pai[v] = u;
                    nivel[v] = nivel[u] + 1;
                    q.push(v);
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
    const vector<int>& vizinhos = (rep == LISTA_ADJ) ? lista_adj[v] : vector<int>();
    for (int u = 0; u < V; u++) {
        if ((rep == MATRIZ_ADJ && matriz_adj[v][u]) || (rep == LISTA_ADJ && find(vizinhos.begin(), vizinhos.end(), u) != vizinhos.end())) {
            if (!visitado[u]) {
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
    const vector<int>& vizinhos = (rep == LISTA_ADJ) ? lista_adj[v] : vector<int>();
    for (int u = 0; u < V; u++) {
        if ((rep == MATRIZ_ADJ && matriz_adj[v][u]) || (rep == LISTA_ADJ && find(vizinhos.begin(), vizinhos.end(), u) != vizinhos.end())) {
            if (!visitado[u]) {
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

    sort(componentes.begin(), componentes.end(), [](const vector<int>& a, const vector<int>& b) {return a.size() > b.size();});
    
    ofstream out(nome_saida);
    out << "# Numero de componentes: " << componentes.size() << "\n";
    for (size_t i = 0; i < componentes.size(); i++) {
        out << "Componente " << i + 1 << " (tam: " << componentes[i].size() << "): ";
        for (int v : componentes[i]) out << v << " ";
        out << "\n";
    }
}

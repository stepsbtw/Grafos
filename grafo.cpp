#include "grafo.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <iomanip>

Grafo::Grafo(int V, Representacao rep) : V(V), A(0), rep(rep) {
    if (rep == LISTA_ADJ) {
        lista_adj.resize(V);
    } else {
        matriz_adj.resize(V, vector<int>(V, 0)); // 0 == sem aresta
    }
}

void Grafo::lerDeArquivo(const string &nome) {
    ifstream in(nome);
    if (!in.is_open()) {
        cerr << "Erro na abertura do arquivo!" << endl;
        return;
    }

    in >> V;
    A = 0;

    if (rep == LISTA_ADJ) {
        lista_adj.resize(V);
    } else {
        matriz_adj.resize(V, vector<int>(V, 0)); 
    }

    int u, v;
    while (in >> u >> v) {
        if (u < 0 || u >= V ||
            v < 0 || v >= V) continue;

        A++;
        if (rep == LISTA_ADJ) {
            lista_adj[u].push_back({v, 1}); // peso = 1
            lista_adj[v].push_back({u, 1}); // undirected
        } else {
            matriz_adj[u][v] = 1;
            matriz_adj[v][u] = 1;
        }
    }

    in.close();
}

void Grafo::salvarDadosGrafo(const string &nomeSaida) {
    ofstream out(nomeSaida);
    if (!out.is_open()) {
        cerr << "Erro na saída!" << endl;
        return;
    }

    // grau médio
    double grauMedio = (double) (2 * A) / V;

    // grau de cada um
    vector<int> grau(V, 0);
    for (int u = 0; u < V; u++) {
        if (rep == LISTA_ADJ) {
            grau[u] = lista_adj[u].size();
        } else {
            for (int v = 0; v < V; v++) {
                if (matriz_adj[u][v] == 1) grau[u]++;
            }
        }
    }

    // distribuição
    vector<int> distribuicao(V + 1, 0);
    for (int u = 0; u < V; u++) {
        distribuicao[grau[u]]++;
    }

    out << "Quantidade de vertices: " << V << endl;
    out << "Quantidade de arestas: " << A << endl;
    out << "Grau medio: " << grauMedio << endl;
    out << "Distribuicao de grau:" << endl;

    for (int g = 0; g <= V; g++) {
        if (distribuicao[g] > 0) {
            out << g << " : " << distribuicao[g] << endl;
        }
    }

    out.close();
}

void Grafo::bfs(int s, const string& nome_saida) {
    vector<int> pai(V, -1);
    vector<int> pre(V, -1);

    bfs_interno(s, pai, pre);

    ofstream saida(nome_saida);
    if (!saida.is_open()) {
        cerr << "Erro ao salvar BFS em " << nome_saida << endl;
        return;
    }

    saida << "BFS a partir do vértice " << s << ":\n";
    for (int v = 0; v < V; ++v) {
        if (pre[v] != -1)
            saida << "Vértice " << v << ": pai = " << pai[v] << ", nível = " << pre[v] << "\n";
    }
    saida.close();
}

void Grafo::bfs_interno(int s, vector<int>& pai, vector<int>& nivel) {
    vector<bool> visitado(V, false);
    queue<int> fila;

    visitado[s] = true;
    nivel[s] = 0;
    fila.push(s);

    while (!fila.empty()) {
        int u = fila.front();
        fila.pop();

        if (rep == LISTA_ADJ) {
            for (const auto& [v, peso] : lista_adj[u]) {
                if (!visitado[v]) {
                    visitado[v] = true;
                    pai[v] = u;
                    nivel[v] = nivel[u] + 1;
                    fila.push(v);
                }
            }
        } else { // MATRIZ_ADJ
            for (int v = 0; v < V; ++v) {
                if (matriz_adj[u][v] != -1 && !visitado[v]) {
                    visitado[v] = true;
                    pai[v] = u;
                    nivel[v] = nivel[u] + 1;
                    fila.push(v);
                }
            }
        }
    }
}


void Grafo::dfs(int s, const string& nome_saida) {
    vector<bool> visitado(V, false);
    vector<int> pai(V, -1);
    vector<int> pre(V, -1), post(V, -1);

    dfs_interno(s, visitado, pai, pre, post);

    ofstream saida(nome_saida);
    if (!saida.is_open()) {
        cerr << "Erro ao salvar DFS em " << nome_saida << endl;
        return;
    }

    saida << "DFS a partir do vértice " << s << ":\n";
    for (int v = 0; v < V; ++v) {
        if (pre[v] != -1)
            saida << "Vértice " << v << ": pai = " << pai[v] << ", pré = " << pre[v] << ", pós = " << post[v] << "\n";
    }
    saida.close();
}

void Grafo::dfs_interno(int v, vector<bool>& visitado, vector<int>& pai, vector<int>& pre, vector<int>& post) {
    static int tempo = 0;
    visitado[v] = true;
    pre[v] = tempo++;

    if (rep == LISTA_ADJ) {
        for (const auto& [vizinho, peso] : lista_adj[v]) {
            if (!visitado[vizinho]) {
                pai[vizinho] = v;
                dfs_interno(vizinho, visitado, pai, pre, post);
            }
        }
    } else { // MATRIZ_ADJ
        for (int u = 0; u < V; ++u) {
            if (matriz_adj[v][u] != -1 && !visitado[u]) {
                pai[u] = v;
                dfs_interno(u, visitado, pai, pre, post);
            }
        }
    }

    post[v] = tempo++;
}

void Grafo::dfs_interno(int v, vector<bool> &visitado, vector<int> &comp) {
    visitado[v] = true;
    comp.push_back(v);

    if (rep == LISTA_ADJ) {
        for (const auto &item : lista_adj[v]) {
            int u = item.first;
            if (!visitado[u]) {
                dfs_interno(u, visitado, comp);
            }
        }
    } else { // MATRIZ_ADJ
        for (int u = 0; u < V; u++) {
            if (matriz_adj[v][u] == 1 && !visitado[u]) {
                dfs_interno(u, visitado, comp);
            }
        }
    }
}

void Grafo::componentesConexos(const string &nomeSaida) {
    vector<bool> visitado(V, false);
    vector<vector<int>> componentes;

    for (int v = 0; v < V; v++) {
        if (visitado[v]) continue;

        vector<int> comp;
        dfs_interno(v, visitado, comp);
        componentes.push_back(comp);
    }

    ofstream out(nomeSaida);
    if (!out.is_open()) {
        cerr << "Erro na saída!" << endl;
        return;
    }

    out << "Quantidade de componentes conexos: " << componentes.size() << endl;

    for (int i = 0; i < (int)componentes.size(); i++) {
        out << "Componente " << (i + 1) << " (tamanho = " << componentes[i].size() << "):" << endl;

        for (int v : componentes[i]) {
            out << v << " "; 
        }
        out << endl;
    }

    out.close();
}
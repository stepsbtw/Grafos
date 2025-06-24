#include "grafo.h"
#include <fstream> // getline
#include <iostream> // cin cout cerr
#include <queue>
#include <algorithm> // numeric_limits, reverse
#include <iomanip> // setprecision
#include <stack>

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

    string linha;
    getline(in, linha); // primeira linha: número de vértices
    V = stoi(linha);
    E = 0;

    if (rep == LISTA_ADJ)
        lista_adj.assign(V, vector<pair<int, double>>());
    else
        matriz_adj.assign(V, vector<double>(V, 0.0));

    tem_pesos = false;
    tem_pesos_negativos = false;

    int u, v;
    double peso;

    while (getline(in, linha)) {
        if (linha.empty()) continue;

        int n = sscanf(linha.c_str(), "%d %d %lf", &u, &v, &peso);

        if (n < 2) continue;         // linha inválida
        if (n == 2) peso = 1.0;      // peso ausente → padrão
        else {
            tem_pesos = true;
            if (peso < 0) tem_pesos_negativos = true;
        }

        if (peso == 0.0) {
            cerr << "Erro: peso 0 invalido." << endl;
            continue;
        }

        if (rep == LISTA_ADJ) {
            lista_adj[u].emplace_back(v, peso);
            lista_adj[v].emplace_back(u, peso);
        } else {
            matriz_adj[u][v] = peso;
            matriz_adj[v][u] = peso;
        }

        E++;
    }

    size_t barra = nome_arquivo.find_last_of("/\\");
    size_t ponto = nome_arquivo.find_last_of(".");
    string base = nome_arquivo.substr(barra + 1, ponto - barra - 1);
    nome_base_saida = "saida/" + base;
}




void Grafo::salvar_estatisticas() {
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
    for (int i = 0; i < V; ++i) grau_medio += graus[i];
    grau_medio /= V;

    vector<int> contagem(V + 1, 0);
    for (int i = 0; i < V; ++i) contagem[graus[i]]++;

    string nome_saida = nome_base_saida + "_estatisticas.txt";
    ofstream out(nome_saida);
    out << "# m = " << V << "\n";
    out << "# n = " << E << "\n";
    out << fixed << setprecision(1) << "# d_medio = " << grau_medio << "\n";

    for (int i = 0; i <= V; i++) {
        if (contagem[i] > 0) {
            out << i << " " << fixed << setprecision(2) << (double)contagem[i] / V << "\n";
        }
    }
}

void Grafo::bfs(int s) {
    vector<int> pai(V, -1), nivel(V, -1);
    bfs_interno(s, pai, nivel);

    string nome_saida = nome_base_saida + "_bfs.txt";
    ofstream out(nome_saida);
    out << "# BFS a partir do vértice " << s << "\n";
    out << "# Vertice | Pai | Nível\n";
    for (int i = 0; i < V; i++) {
        out << i << " " << pai[i] << " " << nivel[i] << "\n";
    }
}


void Grafo::dfs(int s) {
    vector<int> pai(V, -1);
    vector<bool> visitado(V, false);
    dfs_interno(s, visitado, pai);

    string nome_saida = nome_base_saida + "_dfs.txt";
    ofstream out(nome_saida);
    out << "# DFS a partir do vértice " << s << "\n";
    out << "# Vertice Pai\n";
    for (int i = 0; i < V; i++) {
        out << i << " " << pai[i] << "\n";
    }
}

void Grafo::componentes_conexas() {
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

    string nome_saida = nome_base_saida + "_componentes.txt";
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

pair<vector<int>, vector<double>> Grafo::caminho_minimo(int s, int t) {    
    vector<int> pai(V, -1);
    vector<double> dists(V, -1.0);

    if (t < 0 || t >= V) {
        cerr << "Erro: vértice de destino " << t << " está fora dos limites (0 a " << V - 1 << ").\n";
        return {vector<int>(), vector<double>()};
    }

    string nome_saida = nome_base_saida + "_caminho_" + to_string(s) + "_" + to_string(t) + ".txt";
    ofstream out(nome_saida);

    if (tem_pesos_negativos) {
        out << "Erro: Grafo contém pesos negativos. Algoritmo de Dijkstra não é aplicável.\n";
        return {vector<int>(), vector<double>()};
    }

    if (tem_pesos) {
        dijkstra(s, t, pai, dists);
    } else {
        vector<int> nivel(V, -1);
        bfs_interno(s, t, pai, nivel);
        for (int i = 0; i < V; ++i) {
            if (nivel[i] != -1) {
                dists[i] = nivel[i];
            }
        }
    }

    vector<int> caminho;

    if (pai[t] == -1 && s != t) {
        out << "# Caminho mínimo de " << s << " até " << t << "\n";
        out << "Caminho: inexistente\n";
        out << "Distância: infinito\n";
        return {caminho, dists};
    }

    int v = t;
    while (v != -1) {
        caminho.push_back(v);
        v = pai[v];
    }
    reverse(caminho.begin(), caminho.end());

    out << "# Caminho mínimo de " << s << " até " << t << "\n";
    out << "Caminho: ";
    for (int v : caminho) {
        out << v << " ";
    }
    out << "\nDistância: " << dists[t] << "\n";

    return {caminho, dists};
}


pair<vector<vector<int>>, vector<double>> Grafo::caminhos_minimos(int s) {
    vector<int> pai(V, -1);
    vector<double> dists(V, -1.0);

    string nome_saida = nome_base_saida + "_caminhos_" + to_string(s) + ".txt";
    ofstream out(nome_saida);

    if (tem_pesos_negativos) {
        out << "Erro: Grafo contém pesos negativos. Algoritmo de Dijkstra não é aplicável.\n";
        return {vector<vector<int>>(), vector<double>()};
    }

    if (tem_pesos) {
        dijkstra(s, pai, dists);
    } else {
        vector<int> nivel(V, -1);
        bfs_interno(s, pai, nivel);
        for (int i = 0; i < V; ++i) {
            if (nivel[i] != -1) {
                dists[i] = nivel[i];
            }
        }
    }

    vector<vector<int>> caminhos(V);
    for (int t = 0; t < V; ++t) {
        if (pai[t] == -1 && s != t) {
            continue;
        }

        int v = t;
        while (v != -1) {
            caminhos[t].push_back(v);
            v = pai[v];
        }
        reverse(caminhos[t].begin(), caminhos[t].end());
    }

    out << "# Caminhos mínimos a partir de " << s << "\n";
    for (int t = 0; t < V; ++t) {
        if (pai[t] == -1 && s != t) {
            out << t << ": inatingível\n";
        } else {
            out << t << ": ";
            for (int v : caminhos[t]) {
                out << v << " ";
            }
            out << "(distância = " << dists[t] << ")\n";
        }
    }

    return {caminhos, dists};
}

// bfs de um nó para todos
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

// bfs parando no nó destino.
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
                    if (v == t) return;
                }
                
            }
        } else {
            for (int v = 0; v < V; v++) {
                if (matriz_adj[u][v] > 0.0 && !visitado[v]) {
                    visitado[v] = true;
                    pai[v] = u;
                    nivel[v] = nivel[u] + 1;
                    fila.push(v);
                    if(v == t) return;
                }
            }
        }
    }
}

void Grafo::dfs_interno(int s, vector<bool>& visitado, vector<int>& pai) {
    stack<int> pilha;
    pilha.push(s);
    pai[s] = -1;

    while (!pilha.empty()) {
        int v = pilha.top();
        pilha.pop();

        if (visitado[v]) continue;

        visitado[v] = true;

        if (rep == LISTA_ADJ) {
            // Adiciona vizinhos em ordem reversa para simular recursão
            vector< pair<int, double> >& vizinhos = lista_adj[v];
            for (int i = vizinhos.size() - 1; i >= 0; --i) {
                int u = vizinhos[i].first;
                if (!visitado[u]) {
                    pai[u] = v;
                    pilha.push(u);
                }
            }
        } else {
            for (int u = V - 1; u >= 0; --u) {
                if (matriz_adj[v][u] > 0.0 && !visitado[u]) {
                    pai[u] = v;
                    pilha.push(u);
                }
            }
        }
    }
}


void Grafo::dfs_componentes(int s, vector<bool>& visitado, vector<int>& componente) {
    stack<int> pilha;
    pilha.push(s);

    while (!pilha.empty()) {
        int v = pilha.top();
        pilha.pop();

        if (visitado[v]) continue;

        visitado[v] = true;
        componente.push_back(v);

        if (rep == LISTA_ADJ) {
            for (const pair<int, double>& par : lista_adj[v]) {
                int u = par.first;
                if (!visitado[u]) {
                    pilha.push(u);
                }
            }
        } else {
            for (int u = 0; u < V; ++u) {
                if (matriz_adj[v][u] > 0.0 && !visitado[u]) {
                    pilha.push(u);
                }
            }
        }
    }
}







// Dijkstra para todos os vértices a partir de s
void Grafo::dijkstra(int s, vector<int>& pai, vector<double>& dists) {
    dists.assign(V, numeric_limits<int>::max()); // infinito!
    pai.assign(V, -1);
    dists[s] = 0;

    // maxheap
    priority_queue< pair<double, int>, vector< pair<double, int> >, greater< pair<double, int> > > fila;
    fila.push(make_pair(0.0, s));

    while (!fila.empty()) {
        pair<double, int> topo = fila.top();
        fila.pop();
        double dist_u = topo.first;
        int u = topo.second;
        
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
void Grafo::dijkstra(int s, int t, vector<int>& pai, vector<double>& dists) {
    dists.assign(V, numeric_limits<int>::max());
    pai.assign(V, -1);
    dists[s] = 0;

    priority_queue< pair<double, int>, vector< pair<double, int> >, greater< pair<double, int> > > fila;
    fila.push(make_pair(0.0, s));

    while (!fila.empty()) {
        pair<double, int> topo = fila.top();
        fila.pop();
        int dist_u = topo.first;
        int u = topo.second;

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
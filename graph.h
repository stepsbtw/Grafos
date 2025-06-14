#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class Grafo {
public: // funcoes "fachada" -> pro usuario usar
    enum Representacao { LISTA_ADJ, MATRIZ_ADJ };

    Grafo(int V, bool com_pesos = false, Representacao rep = LISTA_ADJ);

    void add_aresta(int u, int v, double peso = 1.0);
    void ler_arquivo(const string& nome_arquivo);
    void salvar_resumo(const string& nome_arquivo);

    // Trocar representação
    void lista_adj();
    void matriz_adj();

    // Algoritmos
    void dfs(int s, const string& nome_saida); // chama dfs interno
    void bfs(int s, const string& nome_saida); // chama bfs interno
    void componentes_conexas(const string& nome_saida); // dfs -> Algoritmo de Tarjan
    void caminho_minimo(int s, int t, const string& nome_saida); // BFS se sem pesos, DJKSTRA se com pesos pra destino
    void caminhos_minimos(int s, const string& nome_saida); // bfs ou djkstra pra todos

private: // pra ninguem mexer na nossa implementacao
    int V;
    bool com_pesos;
    Representacao rep;

    vector<vector<pair<int, double>>> lista_adj;
    vector<vector<double>> matriz_adj;

    void bfs_interno(int v, vector<int>& pai, vector<int>& pre);
    void dfs_interno(int v, vector<bool>& visitado, vector<int>& pai, vector<int>& pre, vector<int>& post);

    void dijkstra(int s, vector<double>& dists, vector<int>& pai);
};

#endif

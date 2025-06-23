#pragma once

#include <vector>
#include <string>
using namespace std;

enum Representacao { LISTA_ADJ, MATRIZ_ADJ };

class Grafo {
private:
    int V, E;
    Representacao rep;

    vector<vector<pair<int, double>>> lista_adj; // grafos esparsos
    vector<vector<double>> matriz_adj; // grafos densos

    bool tem_pesos;
    bool tem_pesos_negativos;

    void bfs_interno(int s, vector<int>& pai, vector<int>& nivel);
    void bfs_interno(int s, int t, vector<int>& pai, vector<int>& nivel);
    void dfs_interno(int v, vector<bool>& visitado, vector<int>& pai, vector<int>& pre, vector<int>& post, int& tempo);
    void dfs_componentes(int v, vector<bool>& visitado, vector<int>& componente);
    void dijkstra(int s, int t, vector<int>& pai, vector<int>& dists);
    void dijkstra(int s, vector<int>& pai, vector<int>& dists);
    

public:
    Grafo(int V, Representacao rep);
    void ler_arquivo(const string& nome_arquivo);
    void salvar_estatisticas(const string& nome_arquivo);
    void bfs(int s, const string& nome_arquivo);
    void dfs(int s, const string& nome_arquivo);
    void componentes_conexas(const string& nome_arquivo);
    pair<vector<int>,vector<int>> caminho_minimo(int s, int t, const string& nome_arquivo);
    pair<vector<vector<int>>,vector<int>> caminhos_minimos(int s, const string& nome_arquivo);
};

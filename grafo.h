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

    void bfs_interno(int s, vector<int>& pai, vector<int>& nivel);
    void dfs_interno(int v, vector<bool>& visitado, vector<int>& pai, vector<int>& pre, vector<int>& post, int& tempo);
    void dfs_componentes(int v, vector<bool>& visitado, vector<int>& componente);

public:
    Grafo(int V, Representacao rep);
    void ler_arquivo(const string& nome_arquivo);
    void salvar_estatisticas(const string& nome_arquivo);
    void bfs(int s, const string& nome_arquivo);
    void dfs(int s, const string& nome_arquivo);
    void componentes_conexas(const string& nome_arquivo);
};

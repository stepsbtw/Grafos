#pragma once

#include <vector>
#include <string>
using namespace std;

enum Representacao { LISTA_ADJ, MATRIZ_ADJ };

class Grafo {
private:
    int V, A;
    Representacao rep;

    vector<vector<int>> lista_adj;
    vector<vector<int>> matriz_adj;

    void bfsInterno(int s, vector<int>& pai, vector<int>& nivel);
    void dfsInterno(int v, vector<bool>& visitado, vector<int>& pai, vector<int>& pre, vector<int>& post, int& tempo);
    void dfsComponentes(int v, vector<bool>& visitado, vector<int>& componente);

public:
    Grafo(int V, Representacao rep);
    void lerDeArquivo(const string& nomeArquivo);
    void salvarEstatisticas(const string& nomeArquivo);
    void bfs(int s, const string& nomeSaida);
    void dfs(int s, const string& nomeSaida);
    void componentesConexos(const string& nomeSaida);
};

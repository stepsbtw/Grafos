#pragma once

#include <iostream>
#include <vector>
#include <list>
using namespace std;

enum Representacao {LISTA_ADJ, MATRIZ_ADJ};

class Grafo {
private:
    int V; // número de vértices
    int A; // número de arestas
    Representacao rep;

    vector<vector<pair<int, int>>> lista_adj;
    vector<vector<int>> matriz_adj;

    void bfs_interno(int s, vector<int> &pai, vector<int> &nivel);
    void dfs_interno(int v, vector<bool> &visitado, vector<int> &pai, vector<int> &pre, vector<int> &post, int &tempo);

public:
    Grafo(int V, Representacao rep);
    void lerDeArquivo(const string &nome);
    void salvarDadosGrafo(const string &nomeSaida);
    void bfs(int s, const string &nomeSaida);
    void dfs(int s, const string &nomeSaida);
    void componentesConexos(const string &nomeSaida);
};
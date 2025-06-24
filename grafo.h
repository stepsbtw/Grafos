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

    bool tem_pesos; // se nao tiver -> peso 1.
    bool tem_pesos_negativos; // se sim, nao posso djisktra!
    string nome_base_saida;

    void bfs_interno(int s, vector<int>& pai, vector<int>& nivel);
    void bfs_interno(int s, int t, vector<int>& pai, vector<int>& nivel);
    void dfs_interno(int v, vector<bool>& visitado, vector<int>& pai);
    void dfs_componentes(int v, vector<bool>& visitado, vector<int>& componente);
    void dijkstra(int s, int t, vector<int>& pai, vector<double>& dists);
    void dijkstra(int s, vector<int>& pai, vector<double>& dists);
    

public:
    Grafo(int V, Representacao rep);
    void ler_arquivo(const string& nome_arquivo);
    void salvar_estatisticas();
    void bfs(int s);
    void dfs(int s);
    void componentes_conexas();
    pair<vector<int>,vector<double>> caminho_minimo(int s, int t);
    pair<vector<vector<int>>, vector<double>> caminhos_minimos(int s);

};

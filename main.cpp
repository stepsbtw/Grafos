#include "grafo.h"
#include <iostream>
#include <cstdio>  // sprintf

int main() {
    for (int i = 0; i < 5; i++) {
        // Processando grafos de lista de adjacência
        char lista_entrada[100], lista_saida_estatisticas[100], lista_saida_bfs[100], lista_saida_dfs[100], lista_saida_componentes[100];
        sprintf(lista_entrada, "entradas/lista_adj/%d.txt", i);
        sprintf(lista_saida_estatisticas, "saidas/lista_adj/%d_estatisticas.txt", i);
        sprintf(lista_saida_bfs, "saidas/lista_adj/%d_bfs.txt", i);
        sprintf(lista_saida_dfs, "saidas/lista_adj/%d_dfs.txt", i);
        sprintf(lista_saida_componentes, "saidas/lista_adj/%d_componentes.txt", i);

        Grafo g_lista(0, LISTA_ADJ);
        g_lista.ler_arquivo(lista_entrada);
        g_lista.salvar_estatisticas(lista_saida_estatisticas);
        g_lista.bfs(0, lista_saida_bfs);
        g_lista.dfs(0, lista_saida_dfs);
        g_lista.componentes_conexas(lista_saida_componentes);

        // Processando grafos de matriz de adjacência
        char matriz_entrada[100], matriz_saida_estatisticas[100], matriz_saida_bfs[100], matriz_saida_dfs[100], matriz_saida_componentes[100];
        sprintf(matriz_entrada, "entradas/matriz_adj/%d.txt", i);
        sprintf(matriz_saida_estatisticas, "saidas/matriz_adj/%d_estatisticas.txt", i);
        sprintf(matriz_saida_bfs, "saidas/matriz_adj/%d_bfs.txt", i);
        sprintf(matriz_saida_dfs, "saidas/matriz_adj/%d_dfs.txt", i);
        sprintf(matriz_saida_componentes, "saidas/matriz_adj/%d_componentes.txt", i);

        Grafo g_matriz(0, MATRIZ_ADJ);
        g_matriz.ler_arquivo(matriz_entrada);
        g_matriz.salvar_estatisticas(matriz_saida_estatisticas);
        g_matriz.bfs(0, matriz_saida_bfs);
        g_matriz.dfs(0, matriz_saida_dfs);
        g_matriz.componentes_conexas(matriz_saida_componentes);
    }

    return 0;
}

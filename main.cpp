#include "grafo.h"
#include <iostream>

int main(){
    int no_inicial = 0;

    Grafo g1(0, LISTA_ADJ);
    g1.ler_arquivo("g1.txt");
    g1.salvar_estatisticas("g1_estatisticas.txt");
    g1.bfs(no_inicial, "g1_bfs.txt");
    g1.dfs(no_inicial, "g1_dfs.txt");
    g1.componentes_conexas("g1_componentes.txt");

    Grafo g2(0, MATRIZ_ADJ);
    g2.ler_arquivo("g1.txt");
    g2.salvar_estatisticas("g2_estatisticas.txt");
    g2.bfs(no_inicial, "g2_bfs.txt");
    g2.dfs(no_inicial, "g2_dfs.txt");
    g2.componentes_conexas("g2_componentes.txt");
}

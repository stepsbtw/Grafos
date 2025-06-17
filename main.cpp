#include "grafo.h"
#include <iostream>

int main() {
    Grafo g(0, LISTA_ADJ); // ou MATRIZ_ADJ
    g.lerDeArquivo("entrada.txt");

    g.salvarEstatisticas("saida_estatisticas.txt");
    g.bfs(0, "saida_bfs.txt");
    g.dfs(0, "saida_dfs.txt");
    g.componentesConexos("saida_componentes.txt");

    return 0;
}

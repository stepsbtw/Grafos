#include "grafo.h"

int main() {
    Grafo g(0, false, Grafo::LISTA_ADJ);
    g.ler_arquivo("entrada.txt");

    g.bfs(0, "saida_bfs.txt");
    g.dfs(0, "saida_dfs.txt");

    return 0;
}

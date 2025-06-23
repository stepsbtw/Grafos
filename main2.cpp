#include "grafo.h"
#include <iostream>

int main(){
    int no_inicial = 0;

    Grafo g(0, LISTA_ADJ);
    g.ler_arquivo("entrada/peso1.txt");
    g.salvar_estatisticas("saida/peso1_estatisticas.txt");
    g.bfs(no_inicial, "saida/peso1_bfs.txt");
    g.dfs(no_inicial, "saida/peso1_dfs.txt");
    g.componentes_conexas("saida/peso1_componentes.txt");
    g.caminho_minimo(no_inicial, 13, "saida/peso1_caminhos.txt");
}

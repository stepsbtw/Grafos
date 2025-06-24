#include "grafo.h"
#include <iostream>

int main(){

    Grafo g(0, LISTA_ADJ);
    g.ler_arquivo("entrada/test.txt");
    g.salvar_estatisticas();
    g.bfs(0);
    g.dfs(0);
    g.componentes_conexas();
    g.caminhos_minimos(0);
    g.caminho_minimo(0, 5);

    Grafo g2(0, LISTA_ADJ);
    g2.ler_arquivo("entrada/test2.txt");
    g2.salvar_estatisticas();
    g2.bfs(0);
    g2.dfs(0);
    g2.componentes_conexas();
    g2.caminhos_minimos(0);
    g2.caminho_minimo(0, 3);

    Grafo g3(0, LISTA_ADJ);
    g3.ler_arquivo("entrada/test3.txt");
    g3.salvar_estatisticas();
    g3.bfs(0);
    g3.dfs(0);
    g3.componentes_conexas();
    g3.caminhos_minimos(0);
    g3.caminho_minimo(0, 13);

}

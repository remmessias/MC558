#include "trilha_euleriana_ra186416.hpp"

#include <iostream>
#include <list>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <unordered_map>

#include "grafo.hpp"

using namespace std;

/**
 * Implementação adaptada do algoritmo de Kosaraju
 * 
 * O algoritmo possui complexidade O(V+E) para 
 * encontrar componentes fortemente conexos, se 
 * houver mais de um CFC, o grafo inteiro não é 
 * fortemente conexo.
 */

/**
 * Função que preenche a pilha com vértices em 
 * ordem de finalização crescente.
 */
void preenche(Grafo G, int v, bool visitados[], stack<int> &pilha) {
    visitados[v] = true;
    list<int>::iterator i;
    
    for (i = G.vizinhos[v].begin(); i != G.vizinhos[v].end(); i++) {
        if (visitados[*i] == false)
            preenche(G, *i, visitados, pilha);
    }
    
    pilha.push(v);
}

/**
 * Função que retorna a transposta do grafo.
 */
Grafo grafoTransposto(Grafo G) {
    Grafo grafo_transposto(G.V,G.M);
    
    for (int v = 0; v < G.V; v++) {
        list<int>::iterator i;
        
        for (i = G.vizinhos[v].begin(); i != G.vizinhos[v].end(); i++) {
            grafo_transposto.vizinhos[*i].push_back(v);
        }
    }

    return grafo_transposto;
}

/**
 * Função que faz busca em profundidade (Depth First 
 * Search - DFS), marcando os vértices como visitados.
 */
void depthFirstSearch(Grafo G, int v, bool visitados[]) {
    visitados[v] = true;
    list<int>::iterator i;
    
    for (i = G.vizinhos[v].begin(); i != G.vizinhos[v].end(); i++) {
        if (visitados[*i] == false)
            depthFirstSearch(G, *i, visitados);
    }
}

/**
 * Função que encontra e identifica a presença de
 * componentes fortemente conectados.
 */
bool verificaComponentes(Grafo G) {
    stack<int> pilha;
    bool *visitados = new bool[G.V]; // Marca os vértices como não visitados

    for (int i = 0; i < G.V; i++) {
        visitados[i] = false;
    }

    // Preenche os vértices na pilha de acordo com tempo de finalização
    for (int i = 0; i < G.V; i++) {
        if (visitados[i] == false) {
            preenche(G, i, visitados, pilha);
        }
    }

    Grafo gt = grafoTransposto(G);

    // Marca os vértices como não visitados para 
    // a segunda Depdth First Search
    for (int i = 0; i < G.V; i++)
        visitados[i] = false;

    int n_vezes = 0; // Número de vezes que foram encontrados Componentes Fortemente Conexos

    // Processa os vértices na ordem da pilhas
    while (!pilha.empty()) {
        int v = pilha.top();
        pilha.pop();

        // Encontra cada componente fortemente conexa
        if (visitados[v] == false) {
            depthFirstSearch(gt, v, visitados);
            n_vezes++;
        }
    }

    // Se apenas uma componente fortemente conexa foi 
    // encontrado, o grafo é inteiro fortemente conexo, 
    // caso contrário, não.
    if (n_vezes == 1)
        return true;

    return false;
}

void encontraCaminho(Grafo G, int* trilha) {
    list <int>* adjacencia = new list<int>[G.V]; // Copia a lista de adjacência do grafo para adjacencia
    list<int>::iterator j;

    for (int i = 0; i < G.V; i++) {
        for (j = G.vizinhos[i].begin(); j != G.vizinhos[i].end();j++) {
            adjacencia[i].push_front(*j);
        }
    }

    vector<int> circuit; // Trilha durante a busca
    unordered_map<int,int> edge_counter; // Número de arestas saindo de um vértice
    stack<int> curr_trail;
    int curr_v,next_v;

    // Contagem do número total de arestas
    for (int i = 0; i < G.V;i++) {
        edge_counter[i] = adjacencia[i].size();
    }

    // Inicializa a busca no vértice 0 e colocamos ele na trilha atual
    curr_v = 0;
    curr_trail.push(0);

    while (!curr_trail.empty()) {
        if (edge_counter[curr_v]) {
            // Verifica se existe qualquer aresta sobrando partindo do vértice atual,
            // adiciona na trilha atual, acha o próximo vértice a partir da aresta, 
            // marca a aresta como indisponível e avança para o próximo vértice.
            curr_trail.push(curr_v);
            next_v = adjacencia[curr_v].back();
            edge_counter[curr_v]--;
            adjacencia[curr_v].pop_back();
            curr_v = next_v;
        }
        else {
            // Encontra o restante da trilha
            circuit.push_back(curr_v);
            curr_v = curr_trail.top();
            curr_trail.pop();
        }
    }

    int k = 0;
    
    // Coloca no vetor trilha a trilha encontrada
    for (int i=circuit.size()-1; i>=0; i--) {
        trilha[k] = circuit[i];
        k++;
    }
}

bool trilha_euleriana(int n, int m, Grafo G, int* origem, int* destino, int* trilha, string &mensagem, int RA) {
    list<int>::iterator j;    
    int k = 0;
    vector<int> entrada(n,0);
    vector<int> saida(n,0);

    // Itera sobre a lista de adjacencia para achar 
    // o grau de entrada e saida
    for (int i = 0; i < n; i++) {
        for (j = G.vizinhos[i].begin(); j != G.vizinhos[i].end();j++) {
            saida[i]++;
            entrada[*j]++;
            k++;
        }
    }

    // Se existir u e V(G) tal que grau de entrada de 
    // u != grau de saida, mostrar "Erro: Existe vértice 
    // inviável." e interromper execução. 
    for (int i = 0; i < n; i++) {
        if (entrada[i] != saida[i]) {
            mensagem = "Erro: Existe vertice inviavel.";
            return false;
        }
    }

    // Se G não for fortemente conexo, mostrar "Erro: 
    // Grafo não eh fortemente conexo." e interromper 
    // conexão.
    if (!(verificaComponentes(G))) {
        mensagem = "Erro: Grafo nao eh fortemente conexo.";
        return false;
    }

    // Imprimir uma trilha fechada euleriana começando e 
    // terminando em v, construida em tempo linear usar o 
    // vetor trilha.
    encontraCaminho(G,trilha);
    
    for (int i = 0; i < (G.M);i++) {
        origem[i] = trilha[i];
        destino[i] = trilha[i+1];
    }

    return true;
}
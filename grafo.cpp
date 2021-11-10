#include <iostream>
#include <list>
#include <algorithm>
#include <string>
#include "grafo.hpp"

using namespace std;

Grafo::Grafo(int V, int M){
	this->V = V; 
    this->M = M;
	vizinhos = new list<int>[V]; 
}

// Criação da aresta (v1, v2) em G.
void Grafo::adicionarAresta(int v1, int v2){
	vizinhos[v1].push_back(v2);
}

// Verifica se existe aresta (v1, v2) em G e retorna true se existe e false caso contrário.
bool Grafo::existeAresta(int v1, int v2){
	if(find(vizinhos[v1].begin(), vizinhos[v1].end(), v2) != vizinhos[v1].end())
		return true;
	return false;
}


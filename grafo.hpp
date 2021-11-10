#ifndef GRAFO_HPP 
#define GRAFO_HPP

using namespace std;

class Grafo{

public:
    std::list<int> *vizinhos; // Lista contendo as arestas
	int V; // Quantidade de vértices
    int M; // Quantidade de arestas

    Grafo();
	Grafo(int V, int M); 
	void adicionarAresta(int v1, int v2); 
	bool existeAresta(int v1, int v2); 
};

#endif
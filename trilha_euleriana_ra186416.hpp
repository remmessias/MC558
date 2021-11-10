#ifndef trilha_euleriana_ra186416
#define trilha_euleriana_ra186416

#include <string.h>
#include <stdbool.h>
#include <list>
#include <string>
#include "grafo.hpp"

using namespace std;

bool trilha_euleriana(int n, int m, Grafo G, int* origem, int* destino, int* trilha, string &mensagem, int RA);

#endif
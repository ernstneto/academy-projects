#ifndef BOLINHAS_H
#define BOLINHAS_H

#include "terrain.h"
#include "types.h"
#include <math.h>

#define MAX_BOLINHAS 50

typedef struct {
    Vertice pos, start, end;
    int verticeAtual;
    int* caminho;
    int tamanhoCaminho;
    float animProgress;
    bool ativa;
} Bolinha;

extern Bolinha bolinhas[MAX_BOLINHAS];

void initBolinhas();
void update(int value);
void drawBolinhas();
int encontrarvizinhoMaisBaixo(int vertice);
void tratarColisoes();
float distanciaEntreBolinhas(Bolinha *a, Bolinha *b);

#endif

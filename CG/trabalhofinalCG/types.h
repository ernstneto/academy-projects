#ifndef TYPES_H
#define TYPES_H

typedef struct {
    float x, y, z;
    int* arestas;
    int num_arestas;
} Vertice;

typedef struct {
    int v1, v2;
    int* triangs;
    int num_triangs;
} Aresta;

typedef struct {
    int v1, v2, v3;
    int a1, a2, a3;
} Triangulo;

#endif

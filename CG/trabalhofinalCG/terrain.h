#ifndef TERRAIN_H
#define TERRAIN_H

#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include "bolinhas.h"
#include "types.h"

#define GRID_SIZE 100

extern Vertice* vertices;
extern Aresta* arestas;
extern Triangulo* triangulos;
extern int totalVertices, totalArestas, totalTriangulos;

void generateTerrain();
void freeTerrainData();
void generateEdgesFromTriangles();
void drawTerrain();
void saveTerrainToOBJ(const char* filename);
void saveTerrainWithTimestamp();

#endif

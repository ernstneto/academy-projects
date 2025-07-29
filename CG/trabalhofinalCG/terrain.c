#include "terrain.h"
#include "utils.h"
#include <stdlib.h>
#include "bolinhas.h"

Vertice* vertices = NULL;
Aresta* arestas = NULL;
Triangulo* triangulos = NULL;
int totalVertices = 0, totalArestas = 0, totalTriangulos = 0;

void generateTerrain() {
    totalVertices = GRID_SIZE * GRID_SIZE;
    totalTriangulos = 2 * (GRID_SIZE-1) * (GRID_SIZE-1);
    vertices = (Vertice*)malloc(totalVertices * sizeof(Vertice));
    triangulos = (Triangulo*)malloc(totalTriangulos * sizeof(Triangulo));
    
    float scale = 0.05f;
    float heightScale = 15.0f;
    float offset = -GRID_SIZE/2.0f;
    for (int z = 0; z < GRID_SIZE; z++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            int idx = z * GRID_SIZE + x;
            vertices[idx].x = x + offset;
            vertices[idx].y = heightScale * perlinNoise(x * scale, z * scale);
            vertices[idx].z = z + offset;
            vertices[idx].num_arestas = 0;
            vertices[idx].arestas = NULL;
        }
    }

    int triIndex = 0;
    for (int z = 0; z < GRID_SIZE-1; z++) {
        for (int x = 0; x < GRID_SIZE-1; x++) {
            int i = z * GRID_SIZE + x;
            
            triangulos[triIndex].v1 = i;
            triangulos[triIndex].v2 = i + 1;
            triangulos[triIndex].v3 = i + GRID_SIZE;
            triIndex++;

            triangulos[triIndex].v1 = i + 1;
            triangulos[triIndex].v2 = i + GRID_SIZE + 1;
            triangulos[triIndex].v3 = i + GRID_SIZE;
            triIndex++;
        }
    }

    totalArestas = 0;
    arestas = (Aresta*)malloc(3 * totalTriangulos * sizeof(Aresta));
    
    for (int i = 0; i < totalTriangulos; i++) {
        int v[3] = {triangulos[i].v1, triangulos[i].v2, triangulos[i].v3};
        
        for (int j = 0; j < 3; j++) {
            int v1 = v[j];
            int v2 = v[(j+1)%3];
            
            bool existe = false;
            for (int k = 0; k < totalArestas; k++) {
                if ((arestas[k].v1 == v1 && arestas[k].v2 == v2) ||
                    (arestas[k].v1 == v2 && arestas[k].v2 == v1)) {
                    existe = true;
                    break;
                }
            }
            
            if (!existe) {
                arestas[totalArestas].v1 = v1;
                arestas[totalArestas].v2 = v2;
                arestas[totalArestas].num_triangs = 0;
                arestas[totalArestas].triangs = NULL;
                totalArestas++;
            }
        }
    }
    
    for (int i = 0; i < totalArestas; i++) {
        int v1 = arestas[i].v1;
        int v2 = arestas[i].v2;
        
        vertices[v1].num_arestas++;
        vertices[v1].arestas = (int*)realloc(vertices[v1].arestas, 
                                          vertices[v1].num_arestas * sizeof(int));
        vertices[v1].arestas[vertices[v1].num_arestas-1] = i;
        
        vertices[v2].num_arestas++;
        vertices[v2].arestas = (int*)realloc(vertices[v2].arestas, 
                                          vertices[v2].num_arestas * sizeof(int));
        vertices[v2].arestas[vertices[v2].num_arestas-1] = i;
    }
}

void freeTerrainData() {
    if (vertices) {
        for (int i = 0; i < totalVertices; i++) {
            if (vertices[i].arestas) free(vertices[i].arestas);
        }
        free(vertices);
        vertices = NULL;
    }
    if (arestas) {
        for (int i = 0; i < totalArestas; i++) {
            if (arestas[i].triangs) free(arestas[i].triangs);
        }
        free(arestas);
        arestas = NULL;
    }
    if (triangulos) {
        free(triangulos);
        triangulos = NULL;
    }
    
    for(int i = 0; i < MAX_BOLINHAS; i++) {
        if(bolinhas[i].caminho) {
            free(bolinhas[i].caminho);
            bolinhas[i].caminho = NULL;
        }
    }
    
    totalVertices = totalArestas = totalTriangulos = 0;
}

void generateEdgesFromTriangles() {
    if (!triangulos || !vertices) return;
    
    totalArestas = 0;
    arestas = (Aresta*)realloc(arestas, 3 * totalTriangulos * sizeof(Aresta));
    if (!arestas) return;
    
    for (int i = 0; i < totalTriangulos; i++) {
        int v[3] = {triangulos[i].v1, triangulos[i].v2, triangulos[i].v3};
        
        for (int j = 0; j < 3; j++) {
            int v1 = v[j];
            int v2 = v[(j+1)%3];
            
            bool existe = false;
            for (int k = 0; k < totalArestas; k++) {
                if ((arestas[k].v1 == v1 && arestas[k].v2 == v2) ||
                    (arestas[k].v1 == v2 && arestas[k].v2 == v1)) {
                    existe = true;
                    break;
                }
            }
            
            if (!existe) {
                if (totalArestas >= 3 * totalTriangulos) {
                    arestas = (Aresta*)realloc(arestas, (totalArestas + 1) * sizeof(Aresta));
                    if (!arestas) return;
                }
                
                arestas[totalArestas].v1 = v1;
                arestas[totalArestas].v2 = v2;
                arestas[totalArestas].num_triangs = 0;
                arestas[totalArestas].triangs = NULL;
                totalArestas++;
            }
        }
    }
    
    for (int i = 0; i < totalVertices; i++) {
        vertices[i].num_arestas = 0;
        free(vertices[i].arestas);
        vertices[i].arestas = NULL;
    }
    
    for (int i = 0; i < totalArestas; i++) {
        int v1 = arestas[i].v1;
        int v2 = arestas[i].v2;
        
        if (v1 < 0 || v1 >= totalVertices || v2 < 0 || v2 >= totalVertices) continue;
        
        vertices[v1].num_arestas++;
        vertices[v1].arestas = (int*)realloc(vertices[v1].arestas, 
                                        vertices[v1].num_arestas * sizeof(int));
        vertices[v1].arestas[vertices[v1].num_arestas-1] = i;
        
        vertices[v2].num_arestas++;
        vertices[v2].arestas = (int*)realloc(vertices[v2].arestas, 
                                        vertices[v2].num_arestas * sizeof(int));
        vertices[v2].arestas[vertices[v2].num_arestas-1] = i;
    }
}

void drawTerrain() {
    GLboolean lightingEnabled;
    glGetBooleanv(GL_LIGHTING, &lightingEnabled);
    
    if (!lightingEnabled) {
        glDisable(GL_LIGHTING);
    }
    
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < totalTriangulos; i++) {
        Vertice* v1 = &vertices[triangulos[i].v1];
        Vertice* v2 = &vertices[triangulos[i].v2];
        Vertice* v3 = &vertices[triangulos[i].v3];
        
        float avgY = (v1->y + v2->y + v3->y) / 3.0f;
        if (avgY > 10.0f) glColor3f(1.0f, 1.0f, 1.0f);
        else if (avgY > 5.0f) glColor3f(0.6f, 0.5f, 0.3f);
        else if (avgY > 0.0f) glColor3f(0.2f, 0.8f, 0.2f);
        else glColor3f(0.1f, 0.1f, 0.8f);

        glVertex3f(v1->x, v1->y, v1->z);
        glVertex3f(v2->x, v2->y, v2->z);
        glVertex3f(v3->x, v3->y, v3->z);
    }
    glEnd();
    
    drawBolinhas();
    
    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
    }
}

void saveTerrainToOBJ(const char* filename) {
     if (!filename || !vertices || !triangulos) return;
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Erro ao criar arquivo OBJ\n");
        return;
    }
    
    for (int i = 0; i < totalVertices; i++) {
        fprintf(file, "v %f %f %f\n", vertices[i].x, vertices[i].y, vertices[i].z);
    }
    
    for (int i = 0; i < totalTriangulos; i++) {
        fprintf(file, "f %d %d %d\n", 
                triangulos[i].v1 + 1, 
                triangulos[i].v2 + 1, 
                triangulos[i].v3 + 1);
    }
    
    fclose(file);
    printf("Terreno salvo como %s\n", filename);
}

void saveTerrainWithTimestamp() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char filename[128];
    
    strftime(filename, sizeof(filename), "terreno_%Y%m%d_%H%M%S.obj", t);
    saveTerrainToOBJ(filename);
}

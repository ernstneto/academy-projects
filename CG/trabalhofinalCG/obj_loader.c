#include "obj_loader.h"
#include "terrain.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    float x, y, z;
} VertexTemp;

typedef struct {
    int v[3];
} FaceTemp;

int loadOBJ(const char* filename) {
    // Abre o arquivo
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", filename);
        return 0;
    }

    // 1. Fase de contagem e alocação inicial
    printf("Fase 1: Contando vértices e faces...\n");
    int vertexCount = 0, faceCount = 0;
    char buffer[1024];

    // Conta vértices e faces primeiro para alocação precisa
    while (fgets(buffer, sizeof(buffer), file)) {
        if (buffer[0] == 'v' && isspace(buffer[1])) vertexCount++;
        else if (buffer[0] == 'f' && isspace(buffer[1])) faceCount++;
    }

    if (vertexCount == 0 || faceCount == 0) {
        fprintf(stderr, "Erro: Arquivo não contém vértices ou faces válidas\n");
        fclose(file);
        return 0;
    }

    // 2. Aloca memória com margem de segurança
    VertexTemp* tempVertices = (VertexTemp*)malloc(vertexCount * sizeof(VertexTemp));
    FaceTemp* tempFaces = (FaceTemp*)malloc(faceCount * sizeof(FaceTemp));
    
    if (!tempVertices || !tempFaces) {
        fprintf(stderr, "Erro: Falha na alocação de memória\n");
        if (tempVertices) free(tempVertices);
        if (tempFaces) free(tempFaces);
        fclose(file);
        return 0;
    }

    // 3. Fase de leitura dos dados
    rewind(file);
    int currentVertex = 0, currentFace = 0;
    int lineNum = 0;
    int warningCount = 0;
    const int maxWarnings = 10;

    printf("Fase 2: Lendo dados...\n");
    while (fgets(buffer, sizeof(buffer), file) && warningCount < maxWarnings) {
        lineNum++;
        char *line = buffer;
        
        // Pula espaços iniciais
        while (isspace(*line)) line++;
        
        // Linha vazia ou comentário
        if (*line == '\0' || *line == '#') continue;

        // Processa vértice
        if (*line == 'v' && isspace(line[1])) {
            if (currentVertex >= vertexCount) {
                if (warningCount++ < maxWarnings) {
                    fprintf(stderr, "Aviso linha %d: Mais vértices do que o contado inicialmente\n", lineNum);
                }
                continue;
            }

            if (sscanf(line + 2, "%f %f %f", 
                      &tempVertices[currentVertex].x,
                      &tempVertices[currentVertex].y,
                      &tempVertices[currentVertex].z) != 3) {
                if (warningCount++ < maxWarnings) {
                    fprintf(stderr, "Aviso linha %d: Formato de vértice inválido\n", lineNum);
                }
                continue;
            }
            currentVertex++;
        }
        // Processa face
        else if (*line == 'f' && isspace(line[1])) {
            if (currentFace >= faceCount) {
                if (warningCount++ < maxWarnings) {
                    fprintf(stderr, "Aviso linha %d: Mais faces do que o contado inicialmente\n", lineNum);
                }
                continue;
            }

            // Suporta formatos: f v1 v2 v3 | f v1/vt1 v2/vt2 v3/vt3 | f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
            int v[3] = {0}, t[3] = {0}, n[3] = {0};
            int readCount = sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                                 &v[0], &t[0], &n[0],
                                 &v[1], &t[1], &n[1],
                                 &v[2], &t[2], &n[2]);

            if (readCount == 9 || readCount == 6) {
                // Formato com texturas/normais
                tempFaces[currentFace].v[0] = v[0] - 1;
                tempFaces[currentFace].v[1] = v[1] - 1;
                tempFaces[currentFace].v[2] = v[2] - 1;
            } 
            else if (sscanf(line + 2, "%d %d %d", &v[0], &v[1], &v[2]) == 3) {
                // Formato simples
                tempFaces[currentFace].v[0] = v[0] - 1;
                tempFaces[currentFace].v[1] = v[1] - 1;
                tempFaces[currentFace].v[2] = v[2] - 1;
            }
            else {
                if (warningCount++ < maxWarnings) {
                    fprintf(stderr, "Aviso linha %d: Formato de face inválido\n", lineNum);
                }
                continue;
            }
            currentFace++;
        }
    }

    if (warningCount >= maxWarnings) {
        fprintf(stderr, "Aviso: Muitos warnings, parando de exibir...\n");
    }

    fclose(file);

    // 4. Verificação de consistência
    printf("Fase 3: Verificando consistência...\n");
    int invalidFaces = 0;
    for (int i = 0; i < currentFace; i++) {
        for (int j = 0; j < 3; j++) {
            if (tempFaces[i].v[j] < 0 || tempFaces[i].v[j] >= currentVertex) {
                if (invalidFaces++ < 5) {
                    fprintf(stderr, "Erro: Face %d referencia vértice inválido %d\n", 
                            i+1, tempFaces[i].v[j]+1);
                }
            }
        }
    }

    if (invalidFaces > 0) {
        fprintf(stderr, "Total de faces inválidas: %d\n", invalidFaces);
        free(tempVertices);
        free(tempFaces);
        return 0;
    }

    // 5. Conversão para estruturas permanentes
    printf("Fase 4: Convertendo estruturas...\n");
    freeTerrainData();
    
    totalVertices = currentVertex;
    totalTriangulos = currentFace;
    
    vertices = (Vertice*)malloc(totalVertices * sizeof(Vertice));
    triangulos = (Triangulo*)malloc(totalTriangulos * sizeof(Triangulo));
    
    if (!vertices || !triangulos) {
        fprintf(stderr, "Erro: Falha na alocação final\n");
        free(tempVertices);
        free(tempFaces);
        return 0;
    }

    // Copia vértices
    for (int i = 0; i < totalVertices; i++) {
        vertices[i].x = tempVertices[i].x;
        vertices[i].y = tempVertices[i].y;
        vertices[i].z = tempVertices[i].z;
        vertices[i].num_arestas = 0;
        vertices[i].arestas = NULL;
    }

    // Copia triângulos
    for (int i = 0; i < totalTriangulos; i++) {
        triangulos[i].v1 = tempFaces[i].v[0];
        triangulos[i].v2 = tempFaces[i].v[1];
        triangulos[i].v3 = tempFaces[i].v[2];
    }

    // 6. Geração de arestas
    printf("Fase 5: Gerando arestas...\n");
    generateEdgesFromTriangles();

    // Limpeza
    free(tempVertices);
    free(tempFaces);

    printf("Carregamento concluído: %d vértices, %d faces\n", totalVertices, totalTriangulos);
    return 1;
}

#include "bolinhas.h"
#include "terrain.h"
#include <stdlib.h>

Bolinha bolinhas[MAX_BOLINHAS];

void initBolinhas() {
    for(int i = 0; i < MAX_BOLINHAS; i++) {
        bolinhas[i].ativa = false;
        bolinhas[i].caminho = NULL;
        bolinhas[i].tamanhoCaminho = 0;
    }
}

float distanciaEntreBolinhas(Bolinha *a, Bolinha *b) {
    float dx = a->pos.x - b->pos.x;
    float dy = a->pos.y - b->pos.y;
    float dz = a->pos.z - b->pos.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

void update(int value) {
	(void)value;
    for(int i = 0; i < MAX_BOLINHAS; i++) {
        if(bolinhas[i].ativa) {
            if(bolinhas[i].animProgress < 1.0f) {
                bolinhas[i].animProgress += 0.05f;
                float progress = bolinhas[i].animProgress * bolinhas[i].animProgress;
                bolinhas[i].pos.x = bolinhas[i].start.x + (bolinhas[i].end.x - bolinhas[i].start.x) * progress;
                bolinhas[i].pos.y = bolinhas[i].start.y + (bolinhas[i].end.y - bolinhas[i].start.y) * progress;
                bolinhas[i].pos.z = bolinhas[i].start.z + (bolinhas[i].end.z - bolinhas[i].start.z) * progress;
            } else {
                int proximo = encontrarvizinhoMaisBaixo(bolinhas[i].verticeAtual);
                if(proximo != -1) {
                    bolinhas[i].start = bolinhas[i].pos;
                    bolinhas[i].end = vertices[proximo];
                    bolinhas[i].end.y += 0.3f;
                    bolinhas[i].animProgress = 0.0f;
                    bolinhas[i].verticeAtual = proximo;

                    bolinhas[i].caminho = realloc(bolinhas[i].caminho, (++bolinhas[i].tamanhoCaminho) * sizeof(int));
                    bolinhas[i].caminho[bolinhas[i].tamanhoCaminho-1] = proximo;
                } else {
                    bolinhas[i].ativa = false;
                }
            }
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(60, update, 0);
}

void drawBolinhas() {
    for(int i = 0; i < MAX_BOLINHAS; i++) {
        if(bolinhas[i].ativa) {
            glPushMatrix();
            glTranslatef(bolinhas[i].pos.x, bolinhas[i].pos.y, bolinhas[i].pos.z);
            glColor3f(1.0, 0.5, 0.0);
            glutSolidSphere(0.4, 20, 20);
            
            glColor3f(1.0, 0.8, 0.0);
            glutSolidSphere(0.2, 15, 15);
            
            glPopMatrix();

            if(bolinhas[i].tamanhoCaminho > 1) {
                glColor3f(1.0, 0.0, 0.0);
                glBegin(GL_LINE_STRIP);
                for(int j = 0; j < bolinhas[i].tamanhoCaminho; j++) {
                    Vertice v = vertices[bolinhas[i].caminho[j]];
                    glVertex3f(v.x, v.y+0.1, v.z);
                }
                glEnd();
            }
        }
    }
}

int encontrarvizinhoMaisBaixo(int vertice) {
    int vizinhoMaisBaixo = -1;
    float menorAltura = vertices[vertice].y;
    for (int i = 0; i < vertices[vertice].num_arestas; i++) {
        Aresta a = arestas[vertices[vertice].arestas[i]];
        int vizinho = (a.v1 == vertice) ? a.v2 : a.v1;
        if (vertices[vizinho].y < menorAltura) {
            menorAltura = vertices[vizinho].y;
            vizinhoMaisBaixo = vizinho;
        }
    }
    return vizinhoMaisBaixo;
}

void tratarColisoes() {
    float raio = 0.4f;
    float forcaRepulsao = 0.05f;
    
    for(int i = 0; i < MAX_BOLINHAS; i++) {
        if(!bolinhas[i].ativa) continue;
        
        for(int j = i+1; j < MAX_BOLINHAS; j++) {
            if(!bolinhas[j].ativa) continue;
            
            float distancia = distanciaEntreBolinhas(&bolinhas[i], &bolinhas[j]);
            if(distancia < 2 * raio) {
                float dx = bolinhas[i].pos.x - bolinhas[j].pos.x;
                float dy = bolinhas[i].pos.y - bolinhas[j].pos.y;
                float dz = bolinhas[i].pos.z - bolinhas[j].pos.z;
                
                float comprimento = sqrt(dx*dx + dy*dy + dz*dz);
                if(comprimento > 0) {
                    dx /= comprimento;
                    dy /= comprimento;
                    dz /= comprimento;
                }
                
                float intensidade = forcaRepulsao * (2 * raio - distancia);
                
                bolinhas[i].pos.x += dx * intensidade;
                bolinhas[i].pos.y += dy * intensidade;
                bolinhas[i].pos.z += dz * intensidade;
                
                bolinhas[j].pos.x -= dx * intensidade;
                bolinhas[j].pos.y -= dy * intensidade;
                bolinhas[j].pos.z -= dz * intensidade;
            }
        }
    }
}

#include "camera.h"
#include "terrain.h"
#include <GL/glut.h>
#include "lighting.h"
#include <float.h>

float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraDistance = GRID_SIZE * 1.5f;
float orthoZoom = 1.0f;
bool showPathInAllViews = true;

void handleKeyboard(unsigned char key, int x, int y) {
    (void)x; (void)y;
    switch(key) {
        case 27: exit(0);  // ESC para sair
        case 'a': cameraAngleY -= 5.0f; break;
        case 'd': cameraAngleY += 5.0f; break;
        case 'w': cameraAngleX -= 5.0f; break;
        case 's': cameraAngleX += 5.0f; break;
        case 'q':
            cameraDistance += ZOOM_SPEED;
            if(cameraDistance > MAX_DISTANCE) cameraDistance = MAX_DISTANCE;
            break;
        case 'e':
            cameraDistance -= ZOOM_SPEED;
            if(cameraDistance < MIN_DISTANCE) cameraDistance = MIN_DISTANCE;
            break;
        case 'p':  // Tecla 'p' para salvar o terreno
            saveTerrainWithTimestamp();
            break;
    }
    glutPostRedisplay();
}

void handleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Configurar a viewport e matrizes de projeção conforme o clique
        int viewportWidth = glutGet(GLUT_WINDOW_WIDTH);
        int viewportHeight = glutGet(GLUT_WINDOW_HEIGHT);
        
        // Determinar qual viewport foi clicado
        int activeViewport = 0; // 0 = perspectiva, 1 = lateral, 2 = superior
        if (x > viewportWidth/2) {
            if (y < viewportHeight/2) {
                activeViewport = 2; // Superior
            } else {
                activeViewport = 1; // Lateral
            }
        }
        
        // Configurar a viewport temporariamente para o picking
        glViewport(
            (activeViewport == 0) ? 0 : viewportWidth/2,
            (activeViewport == 2) ? 0 : (activeViewport == 1) ? viewportHeight/2 : 0,
            (activeViewport == 0) ? viewportWidth/2 : viewportWidth/2,
            (activeViewport == 0) ? viewportHeight : viewportHeight/2
        );
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        if (activeViewport == 0) {
            gluPerspective(45.0f, (float)(viewportWidth/2)/viewportHeight, 1.0f, 2000.0f);
        } else {
            float orthoSize = GRID_SIZE * 1.5f;
            glOrtho(-orthoSize, orthoSize, -orthoSize, orthoSize, -GRID_SIZE*4, GRID_SIZE*4);
        }
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        switch (activeViewport) {
            case 0: // Perspectiva
                gluLookAt(0.0f, GRID_SIZE*0.8f, cameraDistance, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
                glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
                glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);
                break;
            case 1: // Lateral
                gluLookAt(GRID_SIZE*1.5f, GRID_SIZE*0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
                break;
            case 2: // Superior
                gluLookAt(0.0f, GRID_SIZE*1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
                break;
        }
        
        // Obter a posição 3D do clique
        GLdouble modelview[16], projection[16];
        GLint viewport[4];
        GLdouble posX, posY, posZ;
        GLfloat winX, winY, winZ;
        
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);
        
        winX = (float)x;
        winY = (float)viewport[3] - (float)y;
        
        // Obter a profundidade do pixel clicado
        glReadPixels(x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
        
        // Converter para coordenadas 3D
        gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
        
        // Restaurar a viewport original para o display
        glutPostRedisplay();
        
        // Encontrar o vértice mais próximo
        int verticeSelecionado = -1;
        float menorDistancia = FLT_MAX;
        const float raioMaximo = 2.0f;
        
        for (int i = 0; i < totalVertices; i++) {
            float dx = vertices[i].x - posX;
            float dy = vertices[i].y - posY;
            float dz = vertices[i].z - posZ;
            float distancia = dx*dx + dy*dy + dz*dz;
            
            if (distancia < menorDistancia && distancia < raioMaximo*raioMaximo) {
                menorDistancia = distancia;
                verticeSelecionado = i;
            }
        }
        
        if (verticeSelecionado != -1) {
            printf("Vértice selecionado na viewport %d: %d (%.2f, %.2f, %.2f)\n", 
                  activeViewport,
                  verticeSelecionado,
                  vertices[verticeSelecionado].x,
                  vertices[verticeSelecionado].y,
                  vertices[verticeSelecionado].z);
            
            // Adicionar nova bolinha
            for(int i = 0; i < MAX_BOLINHAS; i++) {
                if(!bolinhas[i].ativa) {
                    bolinhas[i].verticeAtual = verticeSelecionado;
                    bolinhas[i].pos = vertices[verticeSelecionado];
                    bolinhas[i].pos.y += 0.3f;  // Eleva um pouco a bolinha
                    bolinhas[i].start = bolinhas[i].pos;
                    bolinhas[i].end = bolinhas[i].pos;
                    bolinhas[i].animProgress = 0.0f;
                    bolinhas[i].ativa = true;
                    
                    if(bolinhas[i].caminho) {
                        free(bolinhas[i].caminho);
                    }
                    bolinhas[i].caminho = malloc(sizeof(int));
                    bolinhas[i].caminho[0] = verticeSelecionado;
                    bolinhas[i].tamanhoCaminho = 1;
                    
                    printf("Bolinha %d criada no vértice %d\n", i, verticeSelecionado);
                    break;
                }
            }
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    
    // 1. Viewport Perspectiva
    glViewport(0, 0, windowWidth/2, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)(windowWidth/2)/windowHeight, 1.0f, 2000.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, GRID_SIZE*0.8f, cameraDistance, 
              0.0f, 0.0f, 0.0f, 
              0.0f, 1.0f, 0.0f);
    
    glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);
    drawTerrain();

    // 2. Viewport Ortográfica Lateral
    glViewport(windowWidth/2, windowHeight/2, windowWidth/2, windowHeight/2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float orthoSize = (GRID_SIZE * 1.2f) / orthoZoom;
    glOrtho(-orthoSize, orthoSize, -orthoSize, orthoSize, -GRID_SIZE*3, GRID_SIZE*3);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(GRID_SIZE*1.5f, GRID_SIZE*0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    glDisable(GL_LIGHTING);
    drawTerrain();

    // 3. Viewport Ortográfica Superior
    glViewport(windowWidth/2, 0, windowWidth/2, windowHeight/2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-orthoSize, orthoSize, -orthoSize, orthoSize, -GRID_SIZE*3, GRID_SIZE*3);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, GRID_SIZE*1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
    glDisable(GL_LIGHTING);
    drawTerrain();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    (void)width; (void)height;
    glutPostRedisplay();
}

#include "terrain.h"
#include "bolinhas.h"
#include "camera.h"
#include "lighting.h"
#include "utils.h"
#include "obj_loader.h"

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>
#include <ctype.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

void init() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    cameraDistance = GRID_SIZE * 2.5f;
    
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0f, (float)WINDOW_WIDTH/WINDOW_HEIGHT, 1.0f, 2000.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Simulacao de Drenagem - Views Multiplas");
    
    printf("\n=== Controles ===\n");
    printf("W/S/A/D: Rotacionar camera\n");
    printf("Q/E: Zoom in/out\n");
    printf("P: Salvar terreno em arquivo OBJ\n");
    printf("Clique esquerdo: Adicionar bolinha\n");
    printf("ESC: Sair\n\n");
    
    init();
    initLighting();
    initBolinhas();
    
    if(argc > 1) {
    	printf("Carregando malha do arquivo: %s\n", argv[1]);
        if(!loadOBJ(argv[1])) {
            printf("Usando terreno procedural como fallback\n");
            initGradients();
            generateTerrain();
        }
    } else {
	    printf("Gerando terreno procedural\n");
        initGradients();
        generateTerrain();
    }

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyboard);
    glutMouseFunc(handleMouse);
    glutReshapeFunc(reshape);
    glutTimerFunc(60, update, 0);
    
    glutMainLoop();
    freeTerrainData();
    return 0;
}

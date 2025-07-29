#include "lighting.h"
#include <GL/glut.h>

GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
GLfloat light_direction[] = {0.0f, 1.0f, 1.0f, 0.0f};

void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Configuração da luz principal (direcional)
    glLightfv(GL_LIGHT0, GL_POSITION, light_direction);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    
    // Configuração do material para melhor reflexão
    GLfloat mat_specular[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat mat_shininess[] = {30.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

#ifndef CAMERA_H
#define CAMERA_H

#include <stdbool.h>
#include "terrain.h"

#define ZOOM_SPEED 5.0f
#define MIN_DISTANCE (GRID_SIZE * 1.5f)
#define MAX_DISTANCE (GRID_SIZE * 5.0f)

extern float cameraAngleX;
extern float cameraAngleY;
extern float cameraDistance;
extern float orthoZoom;
extern bool showPathInAllViews;

void handleKeyboard(unsigned char key, int x, int y);
void handleMouse(int button, int state, int x, int y);
void reshape(int width, int height);
void display();

#endif

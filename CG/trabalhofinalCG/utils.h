#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <stdlib.h>
#include <time.h>

float perlinNoise(float x, float z);
void initGradients();
float lerp(float a, float b, float t);
float fade(float t);
float dotProduct(int ix, int iz, float x, float z);
int hash(int x, int z);

#endif

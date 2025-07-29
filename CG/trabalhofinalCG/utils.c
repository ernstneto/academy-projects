#include "utils.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

float gradients[12][2];

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

int hash(int x, int z) {
    unsigned int key = x * 31 + z;
    key = (key + 0x7ed55d16) + (key << 12);
    key = (key ^ 0xc761c23c) ^ (key >> 19);
    key = (key + 0x165667b1) + (key << 5);
    key = (key + 0xd3a2646c) ^ (key << 9);
    key = (key ^ 0xb55a4f09) ^ (key >> 16);
    return key % 12;
}

void initGradients() {
    srand(time(NULL));
    for (int i = 0; i < 12; i++) {
        float angle = (rand() % 360) * M_PI / 180.0f;
        gradients[i][0] = cos(angle);
        gradients[i][1] = sin(angle);
    }
}

float dotProduct(int ix, int iz, float x, float z) {
    float* g = gradients[hash(ix, iz)];
    float dx = x - ix;
    float dz = z - iz;
    return dx * g[0] + dz * g[1];
}

float perlinNoise(float x, float z) {
    int ix = (int)x;
    int iz = (int)z;
    float fx = x - ix;
    float fz = z - iz;

    float n00 = dotProduct(ix,   iz,   x, z);
    float n10 = dotProduct(ix+1, iz,   x, z);
    float n01 = dotProduct(ix,   iz+1, x, z);
    float n11 = dotProduct(ix+1, iz+1, x, z);

    float u = fade(fx);
    float v = fade(fz);

    return lerp(lerp(n00, n10, u), lerp(n01, n11, u), v);
}

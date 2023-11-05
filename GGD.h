#pragma once

#include <stdio.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

typedef unsigned char GGD_Pixel;

typedef struct {
 GGD_Pixel r;
 GGD_Pixel g;
 GGD_Pixel b;
} pixelData;

typedef struct {
 int pointSize;
 int width;
 int height;
 int posX;
 int posY;
 pixelData *matrix; // The matrix where shit is drawn
} matrix;
matrix specs;

typedef struct {
 int width;
 int height;
 char* name;
 GLFWwindow* window;
} screen;

GLuint createTexture(matrix* w);

void clearBG(matrix* m, pixelData color);

void plot(int x, int y, pixelData color, matrix *m);

void setupScreen(screen* window);

void drawTexture(matrix m);

pixelData* createMatrix(matrix m);

void terminateScreen(matrix* m, screen* s);

int windowClosed(screen* window);

void updateScreen(matrix *m, screen* w);
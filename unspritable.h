#pragma once

#include <stdio.h>
#include "include/GL/freeglut.h"
#include "include/GLFW/glfw3.h"
#include <stdlib.h>

typedef unsigned char GGD_Pixel;

typedef struct {
 unsigned char r;
 unsigned char g;
 unsigned char b;
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

void setActiveMatrix(matrix* m);

void clearBG(pixelData colorm);

void plot(int x, int y, pixelData color);

void DS(int x, int y, int width, int height, pixelData color);

void DL(int x0, int y0, int x1, int y1, int thickness, pixelData color);

void DC(int centerX, int centerY, int radius, pixelData c);

//void drawFPS(int x, int y, int FPS);

int getKey(char letter);

void setupScreen(screen* window);

void drawTexture(matrix m);

pixelData* createMatrix(matrix m);

void terminateScreen(screen* s, matrix* m);

int windowClosed(screen* window);

void updateScreen(screen* w, matrix *m);

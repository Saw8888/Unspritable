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

typedef struct {
 int width;
 int height;
 char* name;
 GLFWwindow* window;
} screen;

screen* s = NULL;
matrix* m = NULL;

void setActiveMatrix(matrix* activeMatrix) {
 m = activeMatrix;
}

GLuint createTexture(matrix* w) {
 GLuint tex;
 glGenTextures(1, &tex);
 glBindTexture(GL_TEXTURE_2D, tex);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w->width, w->height, 0, GL_RGB, GL_UNSIGNED_BYTE, w->matrix);
 glBindTexture(GL_TEXTURE_2D, 0);
 return tex;
}

void clearBG(pixelData color) {
 if (m == NULL) return;
 for (int i = 0; i < m->width * m->height; ++i) {
  m->matrix[i] = color;
 }
}

void plot(int x, int y, pixelData color) {
 if (m == NULL) return;
 int idx = y * m->width + x;
 if (idx > m->width*m->height || idx < 0) return;
 m->matrix[idx] = color;
}

void DL(int x0, int y0, int x1, int y1, int thickness, pixelData color){
 if (m == NULL) return;
 int dx = abs(x1 - x0);
 int dy = abs(y1 - y0);
 int sx = (x0 < x1) ? 1 : -1;
 int sy = (y0 < y1) ? 1 : -1;
 int err = dx - dy;
 int e2, x2, y2;
 int offset = thickness / 2;
 int idx;
 while (1) {
  for (int i = -offset; i <= offset; i++) {
   if (dx > dy) {
    plot(x0,y0+i,color);
   } 
   else {
    plot(x0+i,y0,color);
   }
  }
  if (x0 == x1 && y0 == y1) break;
  e2 = 2 * err;
  if (e2 > -dy) {
   err -= dy;
   x0 += sx;
  }
  if (e2 < dx) {
   err += dx;
   y0 += sy;
  }
 }
}
  
void DC(int xc, int yc, int r, pixelData color) { //Bresenhan's circle drawing algorithm
 int x = 0, y = r;
 int d = 3 - (2 * r);

 while (x <= y) {
  for (int i = -x; i <= x; i++) {
   plot(i + xc, y + yc, color);
   plot(i + xc, -y + yc, color);
  }
  for (int i = -y; i <= y; i++) {
   plot(i + xc, x + yc, color);
   plot(i + xc, -x + yc, color);
  }

  if (d <= 0) {
   d = d + (4 * x) + 6;
  } else {
   d = d + (4 * x) - (4 * y) + 10;
   y = y - 1;
  }
  x = x + 1;
 }
}

void DS(int x, int y, int width, int height, pixelData color){
 if (m == NULL) return;
 int px, py;
 for(px = x; px <= width + x; px++){
  for(py = y; py <= height + y; py++){ 
   plot(px,py,color);
  }
 }
}

/*
void drawFPS(int x, int y, int FPS) { 
 COORD pos = {x, y};
 HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
 SetConsoleCursorPosition(output, pos);
 printf("%d", FPS);
}
*/

int getKey(char letter) {
 if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z')) {
  if (letter >= 'a' && letter <= 'z') {
   letter -= 32;
  }

  int key = GLFW_KEY_A + (letter - 'A');
  return glfwGetKey(s->window, key) == GLFW_PRESS;
 }
 return 0;
}

void setupScreen(screen* window) {
 if (!glfwInit()) {
  exit(EXIT_FAILURE);
 }
 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
 window->window = glfwCreateWindow(window->width, window->height, window->name, NULL, NULL);
 if (!window->window) {
  glfwTerminate();
  exit(EXIT_FAILURE);
 }
 glfwMakeContextCurrent(window->window);
 glfwSwapInterval(0);
 s = window;

 int bufferWidth, bufferHeight;
 glfwGetFramebufferSize(window->window, &bufferWidth, &bufferHeight);
 glViewport(0, 0, bufferWidth, bufferHeight);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(0.0f, bufferWidth, bufferHeight, 0.0f, -1.0f, 1.0f);
 glMatrixMode(GL_MODELVIEW);
 glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void drawTexture(matrix m) {
 glBindTexture(GL_TEXTURE_2D, createTexture(&m));
 glEnable(GL_TEXTURE_2D);
 glBegin(GL_QUADS);
 glTexCoord2i(0, 0); glVertex2i(m.posX, m.posY);
 glTexCoord2i(0, 1); glVertex2i(m.posX, m.posY + m.height * m.pointSize);
 glTexCoord2i(1, 1); glVertex2i(m.posX + m.width * m.pointSize, m.posY + m.height * m.pointSize);
 glTexCoord2i(1, 0); glVertex2i(m.posX + m.width * m.pointSize, m.posY);
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, 0);
}

pixelData* createMatrix(matrix *m) {
 return (pixelData*)malloc(m->height * m->width * sizeof(pixelData));
}

void terminateScreen(screen* s, matrix* m){
 glfwDestroyWindow(s->window);
 glfwTerminate();
 free(m->matrix);
}

int windowClosed(screen* window){
 return glfwWindowShouldClose(window->window);
}

void updateScreen(screen* w, matrix* m){
 glClear(GL_COLOR_BUFFER_BIT);
 drawTexture(*m);
 glfwSwapBuffers(w->window);
 glfwPollEvents();
}

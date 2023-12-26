#include "GGD.h"

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

void clearBG(matrix* m, pixelData color) {
 for (int i = 0; i < m->width * m->height; ++i) {
  m->matrix[i] = color;
 }
}

void plot(int x, int y, pixelData color, matrix *m) {
 int idx = y * m->width + x;
 m->matrix[idx].r = color.r;
 m->matrix[idx].g = color.g;
 m->matrix[idx].b = color.b;
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

pixelData* createMatrix(matrix m) {
 return (pixelData*)malloc(m.height * m.width * sizeof(pixelData));
}

void terminateScreen(matrix* m, screen* s){
 glfwDestroyWindow(s->window);
 glfwTerminate();
 free(m->matrix);
}

int windowClosed(screen* window){
 return glfwWindowShouldClose(window->window);
}

void updateScreen(matrix *m, screen* w){
 glClear(GL_COLOR_BUFFER_BIT);
 drawTexture(*m);
 glfwSwapBuffers(w->window);
 glfwPollEvents();
}

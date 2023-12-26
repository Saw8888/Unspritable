#include "GGD.h"

#define pixelScale 4
#define GLW 1000
#define GLH 500
#define SW GLW/pixelScale+2
#define SH GLH/pixelScale+2

int main(int argc, char** argv) {
 matrix specs;
 screen dimensions;
 dimensions.name = "Main";
 dimensions.width = GLW;
 dimensions.height = GLH;

 setupScreen(&dimensions);

 int textureWidth = dimensions.width / pixelScale; // Adjust texture size based on pixel scale
 int textureHeight = dimensions.height / pixelScale;

 specs.posX = 0;
 specs.posY = 0;
 specs.height = textureHeight;
 specs.width = textureWidth;
 specs.pointSize = pixelScale;
 specs.matrix = createMatrix(specs);

 pixelData greenColor = {0, 255, 0}; // Define green color

 while (!windowClosed(&dimensions)) {
  clearBG(&specs, greenColor);

  updateScreen(&specs, &dimensions);

 }

 terminateScreen(&specs,&dimensions);
 return 0;
}

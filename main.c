#include "src/GGD.h"
#include <math.h>

#define pixelScale 1
#define GLW 1000
#define GLH 500
#define PI 3.1415
#define mapX 8
#define mapY 8

float pX = 10;
float pY = 10;
float pDeltaX;
float pDeltaY;
float pAngle;

unsigned int map[mapY][mapX] = {{1,1,1,1,1,1,1,1},
                                {1,0,0,0,0,0,0,1},
                                {1,0,1,0,0,0,0,1},
                                {1,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,0,1},
                                {1,0,0,0,0,0,0,1},
                                {1,1,1,1,1,1,1,1}};

void drawMap(){
 for(int y=0;y<mapY;y++){
  for(int x=0;x<mapX;x++){
   if(map[y][x] == 1){
    DS(x*64,y*64,60,60,(pixelData){255,255,255});
   }
   else{
    DS(x*64,y*64,60,60,(pixelData){100,100,100});
   }
  }
 }
}

void movePlayer(){
 if (getKey('w')){
   pY+=pDeltaY*5;
   pX+=pDeltaX*5;
  }
 if (getKey('s')){
  pX-=pDeltaX*5;
  pY-=pDeltaY*5;
 }
 if (getKey('d')){
  pAngle+=0.05;
  if(pAngle > 2*PI) pAngle -= 2*PI;
  pDeltaX = cos(pAngle);
  pDeltaY = sin(pAngle);
 }
 if (getKey('a')){
  pAngle-=0.05;
  if(pAngle < 0) pAngle += 2*PI;
  pDeltaX = cos(pAngle);
  pDeltaY = sin(pAngle);
 }
}

int main(int argc, char** argv) {
 matrix specs;
 screen dimensions;
 dimensions.name = "Main";
 dimensions.width = GLW;
 dimensions.height = GLH;

 setupScreen(&dimensions);

 specs.posX = 0;
 specs.posY = 0;
 specs.height = GLH;
 specs.width = GLW;
 specs.pointSize = pixelScale;
 specs.matrix = createMatrix(specs);

 setActiveMatrix(&specs);

 while (!windowClosed(&dimensions)) {
  clearBG((pixelData){0,0,0});

  movePlayer();

  drawMap();
  DL(pX,pY,pX+pDeltaX*30,pY+pDeltaY*30,5,(pixelData){255,0,255});
  DC(pX,pY,10,(pixelData){255,0,0});

  updateScreen(&dimensions, &specs);
 }
 terminateScreen(&dimensions, &specs);
 return 0;
}

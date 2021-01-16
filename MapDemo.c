#include <stdio.h>
#include <conio.h>
#include <c64.h>
#include <cbm_petscii_charmap.h>
#include <peekpoke.h>
#include <joystick.h>

#include "c64_bitmapmode.h"
//#link "c64_bitmapmode.c"

typedef unsigned char byte;
//Map Data
byte mapHeight = 64;
byte mapWidth = 64;
byte mapData[64][64];
//Viewport
byte viewportPosX = 2;
byte viewportPosY = 2;
byte viewportWidth = 20;
byte viewportHeight = 20;
int viewportMemYOffset;
int ViewportMemPos;
int ColorMemPos;
//Camera Position
int offsetX, offsetY = 0;
int x, y, a, b = 0;
//Tile Data
//
//
//Color Palette
byte ColorPalette[64];

void rasterWait(unsigned char line) {
  while (VIC.rasterline < line) ;
}

void CheckInput()
{
  char joy;
  joy = joy_read(0);
  if (JOY_UP(joy)) offsetY-=2;
  if (JOY_LEFT(joy)) offsetX-=2;
  if (JOY_RIGHT(joy)) offsetX+=2;
  if (JOY_DOWN(joy)) offsetY+=2; 
}

void DrawScreen()
{
  //Add the offset to position the first character in the viewport
  ViewportMemPos = 0x0400 + viewportMemYOffset;
  ColorMemPos = 0xD800 + viewportMemYOffset;
  
  if (offsetX > mapWidth)
    offsetX -= mapWidth;
  if (offsetX <= 0)
    offsetX += mapWidth;
  
  if (offsetY > mapHeight)
    offsetY -= mapHeight;
  if (offsetY <= 0)
    offsetY += mapHeight;
  
  a = offsetX;
  b = offsetY;
  
  //y = viewportHeight;
  for(y = 0; y < viewportHeight; y++)
  //while(y--)
  {
    //Wrap the map data y reference
    if (b == mapHeight)
        b = 0;
    if (b < 0)
      b +=mapHeight;
    
    //x = viewportWidth;
    for(x = 0; x < viewportWidth; x++)
    //while(x--)
    {
      //Wrap the map data X reference
      if (a == mapWidth)
          a = 0;
      if (a < 0)
          a += mapWidth;
      
      //cputcxy(x + viewportPosX, y + viewportPosY, mapData[a][b]);
      //POKE(viewportAddress + x + 40*(y + viewportPosY), mapData[a][b]);
      POKE(ViewportMemPos + x + viewportPosX, mapData[a][b]);
      POKE(ColorMemPos + x + viewportPosX, ColorPalette[mapData[a][b]]);
      
      //POKE(0xD800 + x + viewportPosX + 40 * y + 40* viewportPosY, mapData[a][b]);
      
      a++;
    }
    ViewportMemPos += 40;
    ColorMemPos += 40;
    a = offsetX;
    b++;
  }
}

void main(void)
{
  int i;
  
  byte grass = 160;
  byte water = 230;
  byte signpost = 235;
  
  viewportMemYOffset = viewportPosY * 40;
  joy_install (joy_static_stddrv);
  clrscr();  
  
  setcolortextmode();
  
  //Init map data
  for(y = 0; y < mapHeight; y++)
    {
      for(x = 0; x < mapWidth; x++)
      {
        mapData[x][y] = x * y;
      }
    }  
  mapData[4][4] = grass;
  mapData[5][4] = grass;
  mapData[6][4] = grass;
  mapData[7][4] = grass;
  mapData[4][5] = grass;
  mapData[5][5] = signpost;
  mapData[6][5] = grass;
  mapData[7][5] = grass;
  mapData[4][6] = grass;
  mapData[5][6] = grass;
  mapData[6][6] = grass; 
  mapData[7][6] = grass;
  mapData[4][7] = grass;
  mapData[5][7] = grass;
  mapData[6][7] = grass; 
  mapData[7][7] = grass;
  
  for (i = 0; i < 8; i++)
    ColorPalette[i] = i;
  
  ColorPalette[water] = 6;
  ColorPalette[grass] = 5;
  ColorPalette[signpost] = 1;
  
  while(1)
  {
    //rasterWait(255);
    CheckInput();
    DrawScreen();
  }
}
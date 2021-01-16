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
byte viewportWidth = 8;
byte viewportHeight = 8;
int viewportMemYOffset;
int ViewportMemPos;
int ColorMemPos;
//Camera Position
int offsetX, offsetY = 0;
int x, y, a, b = 0;
byte moved = 0;
//Tile Data
struct Tile{
  byte chars[4];
  byte colors[4];
  unsigned int blocked:4;
  unsigned int trigger:4;
} tiles[16];
//Color Palette
byte ColorPalette[256];

void rasterWait(unsigned char line) {
  while (VIC.rasterline < line) ;
}

void CheckInput()
{
  int movement = 1;
  char joy;
  
  moved = 0;
  joy = joy_read(0);
  if (JOY_UP(joy)) {offsetY-=movement; moved = 1;}
  if (JOY_LEFT(joy)) {offsetX-=movement; moved = 1;}
  if (JOY_RIGHT(joy)) {offsetX+=movement; moved = 1;}
  if (JOY_DOWN(joy)) {offsetY+=movement; moved = 1;}
}

void DrawTile(byte index, byte xpos, byte ypos)
{
  int memoffset = xpos + 40 * ypos + (viewportPosX) + (40 * viewportPosY);
  POKE(0x0400 + memoffset, tiles[index].chars[0]);
  POKE(0x0400 + memoffset + 1, tiles[index].chars[1]);
  POKE(0x0400 + memoffset + 40, tiles[index].chars[2]);
  POKE(0x0400 + memoffset + 41, tiles[index].chars[3]);
  
  POKE(0xD800 + memoffset, tiles[index].colors[0]);
  POKE(0xD800 + memoffset + 1, tiles[index].colors[1]);
  POKE(0xD800 + memoffset + 40, tiles[index].colors[2]);
  POKE(0xD800 + memoffset + 41, tiles[index].colors[3]);
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
      
      //POKE(ViewportMemPos + x + viewportPosX, mapData[a][b]);
      //POKE(ColorMemPos + x + viewportPosX, ColorPalette[mapData[a][b]]);
      DrawTile(mapData[a][b], x * 2, y*2);
      
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
  
  byte grass = 2;
  byte water = 0;
  byte signpost = 1;
  
  viewportMemYOffset = viewportPosY * 40;
  joy_install (joy_static_stddrv);
  clrscr();  
  
  setcolortextmode();
  
  //Init map data
  for(y = 0; y < mapHeight; y++)
    {
      for(x = 0; x < mapWidth; x++)
      {
        mapData[x][y] = 0;
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
  
  for (i = 0; i < 256; i++)
    ColorPalette[i] = i;
  
  ColorPalette[water] = 6;
  ColorPalette[grass] = 5;
  ColorPalette[signpost] = 1;
  
  tiles[0].chars[0] = '1';
  tiles[0].chars[1] = '2';
  tiles[0].chars[2] = '3';
  tiles[0].chars[3] = '4';
  tiles[0].colors[0] = 14;
  tiles[0].colors[1] = 6;
  tiles[0].colors[2] = 6;
  tiles[0].colors[3] = 14;
  
  tiles[1].chars[0] = '1';
  tiles[1].chars[1] = '2';
  tiles[1].chars[2] = '3';
  tiles[1].chars[3] = '4';
  tiles[1].colors[0] = 1;
  tiles[1].colors[1] = 1;
  tiles[1].colors[2] = 15;
  tiles[1].colors[3] = 15;
  
  tiles[2].chars[0] = '1';
  tiles[2].chars[1] = '2';
  tiles[2].chars[2] = '3';
  tiles[2].chars[3] = '4';
  tiles[2].colors[0] = 7;
  tiles[2].colors[1] = 13;
  tiles[2].colors[2] = 13;
  tiles[2].colors[3] = 7;
  
  DrawScreen();
  while(1)
  {
    CheckInput();
    if(moved)
      DrawScreen();
  }
}
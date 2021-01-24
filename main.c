#include <stdio.h>
#include <conio.h>
#include <c64.h>
#include <cbm_petscii_charmap.h>
#include <peekpoke.h>
#include <joystick.h>

#include "c64_bitmapmode.h"
//#link "c64_bitmapmode.c"

#include "map.h"
//#link "map.c"


#include "common.h"
//#link "common.c"

//Map Data
byte mapHeight = 64;
byte mapWidth = 64;
byte mapData[64][64];

//Viewport
byte viewportPosX = 2;
byte viewportPosY = 2;
byte viewportWidth = 10;
byte viewportHeight = 10;
int viewportOrigin = 0x0400;
int colorOrigin = 0xD800;

//Camera Position
int offsetX, offsetY = 0;
int x, y, a, b, i = 0;
byte moved = 0;
char joy = 0;

//Color Palette
byte ColorPalette[256];

void rasterWait(unsigned char line) {
  while (VIC.rasterline < line) ;
}

void CheckInput()
{
  if (joy != joy_read(0))
  {
    int movement = 1;  
    moved = 0;
    joy = joy_read(0);
    if (JOY_UP(joy)) {offsetY-=movement; moved = 1;}
    if (JOY_LEFT(joy)) {offsetX-=movement; moved = 1;}
    if (JOY_RIGHT(joy)) {offsetX+=movement; moved = 1;}
    if (JOY_DOWN(joy)) {offsetY+=movement; moved = 1;}
  }
}

//Tile Data
struct Tile
{
  byte chars[4];
  byte colors[4];
  unsigned int blocked:4;
  unsigned int trigger:4;
} tiles[16];

void DrawTile(byte index, byte xpos, byte ypos)
{
  //Note: 
  //could speed up if memoffset is solved 
  //once per line instead of per tile
  
  int memoffset = xpos + COLS * ypos;
  POKE(viewportOrigin + memoffset, tiles[index].chars[0]);
  POKE(viewportOrigin + memoffset + 1, tiles[index].chars[1]);
  POKE(viewportOrigin + memoffset + COLS, tiles[index].chars[2]);
  POKE(viewportOrigin + memoffset + COLS + 1, tiles[index].chars[3]);
  
  POKE(colorOrigin + memoffset, tiles[index].colors[0]);
  POKE(colorOrigin + memoffset + 1, tiles[index].colors[1]);
  POKE(colorOrigin + memoffset + COLS, tiles[index].colors[2]);
  POKE(colorOrigin + memoffset + COLS + 1, tiles[index].colors[3]);
}

void InitializeMapData()
{
  byte grass = 2;
  byte water = 0;
  byte signpost = 1;
  
  viewportOrigin += (viewportPosX + 40 * viewportPosY);
  colorOrigin += (viewportPosX + 40 * viewportPosY);
  
  //Init Tileset
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
  
  //Init map data
  for(y = 0; y < mapHeight; y++)
    {
      for(x = 0; x < mapWidth; x++)
      {
        mapData[x][y] = water;
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
}

void DrawScreen()
{  
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
  
  for(y = 0; y < viewportHeight; y++)
  {
    //Wrap the map data y reference
    if (b == mapHeight)
        b = 0;
    if (b < 0)
      b +=mapHeight;
    
    for(x = 0; x < viewportWidth; x++)
    {
      //Wrap the map data X reference
      if (a == mapWidth)
          a = 0;
      if (a < 0)
          a += mapWidth;
      
      DrawTile(mapData[a][b], x * 2, y*2);
      
      a++;
    }
    
    a = offsetX;
    b++;
  }
}

void main(void)
{ 
  joy_install (joy_static_stddrv);
  clrscr();  
  
  setcolortextmode();
  
  InitializeMapData();
  DrawScreen();
  
  while(1)
  {
    CheckInput();
    
    if(moved)
    {
      DrawScreen();
    }
  }
}
#include <stdio.h>
#include <conio.h>
#include <c64.h>
#include <cbm_petscii_charmap.h>
#include <peekpoke.h>

#include "bitwiseops.h"
#include "c64_bitmapmode.h"
#include "common.h"
#include "input.h"

//Map Data (set these all to the same number)
int mapHeight = 32;
int mapWidth = 32;
byte mapData[32][32];
bool charsDrawn[32][32];

//Viewport
byte viewportPosX = 2;
byte viewportPosY = 2;
byte viewportWidth = 9;
byte viewportHeight = 9;
int viewportOrigin = 0x0400;
int colorOrigin = 0xD800;

//Camera Position
int offsetX, offsetY = 0;
int x, y, a, b, i = 0;
int cameraOffsetX = 0;
int cameraOffsetY = 0;

//Color Palette
byte ColorPalette[256];

byte moved = 0;

//Tile Data
struct Tile
{
  byte chars[4];
  byte colors[4];
  byte blocked;
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

struct Character
{
  byte chars[4];
  byte colors[4];
  int posX;
  int posY;
  bool visible;
} characters[16];

void DrawCharacter(byte index, int xpos, int ypos)
{
  {
    int memoffset = xpos + COLS * ypos;
    POKE(viewportOrigin + memoffset, characters[index].chars[0]);
    POKE(viewportOrigin + memoffset + 1, characters[index].chars[1]);
    POKE(viewportOrigin + memoffset + COLS, characters[index].chars[2]);
    POKE(viewportOrigin + memoffset + COLS + 1, characters[index].chars[3]);
  
    POKE(colorOrigin + memoffset, characters[index].colors[0]);
    POKE(colorOrigin + memoffset + 1, characters[index].colors[1]);
    POKE(colorOrigin + memoffset + COLS, characters[index].colors[2]);
    POKE(colorOrigin + memoffset + COLS + 1, characters[index].colors[3]);
  }
}

void ClampOffset()
{
  if (offsetX >= mapWidth)
    offsetX = 0;
  if (offsetX < 0)
    offsetX = mapWidth - 1;
  
  if (offsetY >= mapHeight)
    offsetY = 0;
  if (offsetY < 0)
    offsetY = mapHeight - 1;
}

void CameraFollow(byte index)
{  
  offsetX = characters[index].posX;
  offsetY = characters[index].posY;
  
    for(x = 0; x < cameraOffsetX; x++)
    {
      offsetX--;
      ClampOffset();
    }
  
  for(y = 0; y < cameraOffsetY; y++)
    {
      offsetY--;
      ClampOffset();
    }
}

byte GetWrappedX(int xPos)
{ 
  int tempX = xPos - offsetX;
  
  if (xPos < offsetX)
    tempX += mapWidth;

  return tempX;
}

byte GetWrappedY(byte YPos)
{
  int tempY = YPos - offsetY;
  
  if (YPos < offsetY)
    tempY += mapHeight;

  return tempY;
}

void DrawChar(byte index)
{
  if(characters[index].visible)
  {
  byte posx = GetWrappedX(characters[index].posX);
  byte posy = GetWrappedY(characters[index].posY);
  
  if (posx < viewportWidth)
    if (posy < viewportHeight)
      DrawCharacter(index, posx * 2, posy * 2);
  charsDrawn[posx][posy] = true;
  }
}

void BlankCharsDrawn()
{
  for(y = 0; y < viewportHeight; y++)
    for (x = 0; x < viewportWidth; x++)
      charsDrawn[x][y] = false;
}

void InitializeMapData()
{
  byte grass = 2;
  byte water = 0;
  byte signpost = 1;
  
  viewportOrigin += (viewportPosX + COLS * viewportPosY);
  colorOrigin += (viewportPosX + COLS * viewportPosY);
  
  cameraOffsetX = viewportWidth / 2;
  cameraOffsetY = viewportHeight / 2;
  
  BlankCharsDrawn();
  
  //Init Tileset
  tiles[0].chars[0] = '1';
  tiles[0].chars[1] = '2';
  tiles[0].chars[2] = '3';
  tiles[0].chars[3] = '4';
  tiles[0].colors[0] = 14;
  tiles[0].colors[1] = 6;
  tiles[0].colors[2] = 6;
  tiles[0].colors[3] = 14;
  
  //Signpost
  tiles[1].chars[0] = '1';
  tiles[1].chars[1] = '2';
  tiles[1].chars[2] = '3';
  tiles[1].chars[3] = '4';
  tiles[1].colors[0] = 1;
  tiles[1].colors[1] = 1;
  tiles[1].colors[2] = 15;
  tiles[1].colors[3] = 15;
  tiles[1].blocked = 0;
  WriteBit(&tiles[1].blocked, 0);
  WriteBit(&tiles[1].blocked, 2);  
  
  tiles[2].chars[0] = '1';
  tiles[2].chars[1] = '2';
  tiles[2].chars[2] = '3';
  tiles[2].chars[3] = '4';
  tiles[2].colors[0] = 7;
  tiles[2].colors[1] = 13;
  tiles[2].colors[2] = 13;
  tiles[2].colors[3] = 7;
  
  //Init Characters
  for (i = 0; i < 16; i++)
  {
    characters[i].chars[0] = i;
    characters[i].chars[1] = i;
    characters[i].chars[2] = i;
    characters[i].chars[3] = i;
    characters[i].colors[0] = i+4;
    characters[i].colors[1] = i+1;
    characters[i].colors[2] = i+2;
    characters[i].colors[3] = i+3;
    characters[i].posX = i;
    characters[i].posY = i;
    characters[i].visible = false;
  }
    characters[0].visible = true;
  
  
  
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
  
  mapData[0][0] = signpost;
  mapData[0][7] = signpost;
  mapData[7][0] = signpost;
  mapData[7][7] = signpost;
  
  for (i = 0; i < 256; i++)
    ColorPalette[i] = i;
  
  ColorPalette[water] = 6;
  ColorPalette[grass] = 5;
  ColorPalette[signpost] = 1;
}

void MapUpdate()
{
  tiles[0].chars[0]++;
}
void DrawMap()
{
  bool charIndexDrawn[16];
  byte charactersLeft = 16;
  
  CameraFollow(0);
  BlankCharsDrawn();

  for (i = 0; i < charactersLeft; i++)
    charIndexDrawn[i] = false;
  
  ClampOffset();
  
  a = offsetX;
  b = offsetY;
  
  for(i = 0; i < 16; i++)
    DrawChar(i);
  
  for(y = 0; y < viewportHeight; y++)
  {
    //Wrap the map data y reference
    if (b == mapHeight)
        b = 0;
    if (b < 0)
      b +=mapHeight;
    
    for(x = 0; x < viewportWidth; x++)
    {
      bool charDrawn = false;
      
      //Wrap the map data X reference
      if (a == mapWidth)
          a = 0;
      if (a < 0)
          a += mapWidth;
      
      
      /*if (charactersLeft > 0)
      for (i = 0; i < 16; i++)
      {
        if (!charIndexDrawn[i])
        {
          
          
          if(characters[i].posX == a)
            if(characters[i].posY == b)
            {
              DrawCharacter(i, x * 2, y * 2);
              if (characters[i].visible)
                charDrawn = true;
              charIndexDrawn[i] = true;
              charactersLeft--;
            }
        }
      }
      
      if(!charDrawn)*/
      
      /*for (i = 0; i < 16; i++)
        if (characters[i].visible)
          if(characters[i].posX == a)
            if(characters[i].posY == b)
              charDrawn = true;*/
      if (!charsDrawn[x][y])
        DrawTile(mapData[a][b], x*2, y*2);
      
      a++;
    }
    
    a = offsetX;
    b++;
  }
  
  
  for (i = 0; i < 34; i++)
    printf("\b");
  printf("\rchrpos x %i", characters[0].posX);
  printf("\r chrpos y %i", characters[0].posY);
  printf("\r byte   y %i", ReadBit(characters[0].posY, 0));
  
}

int wrapX(int posX)
{
  if (posX >= mapWidth)
    posX = 0;
  if (posX < 0)
    posX = mapWidth - 1;  
  
  return posX;
}

int wrapY(int posY)
{
  if (posY >= mapHeight)
    posY = 0;
  if (posY < 0)
    posY = mapHeight - 1;  
  
  return posY;
}

bool CheckCollision(byte charIndex, byte Direction)
{
  int xPos = characters[charIndex].posX;
  int yPos = characters[charIndex].posY;
  
  if(ReadBit(tiles[mapData[xPos][yPos]].blocked, Direction))
    return true;
  
  switch (Direction)
  {
    case 0:
      yPos -= 1;
      yPos = wrapY(yPos);
      Direction = 1;
      break;
    case 1:
      yPos += 1;
      yPos = wrapY(yPos);
      Direction = 0;
      break;
    case 2:
      xPos -= 1;
      xPos = wrapX(xPos);
      Direction = 3;
      break;
    case 3:
      xPos += 1;
      xPos = wrapX(xPos);
      Direction = 2;
      break;
    default:
      return false;
  }
  
  if(ReadBit(tiles[mapData[xPos][yPos]].blocked, Direction))
    return true;
  
  return false;
}

void MoveCharacter(byte index, byte direction, bool cameraUpdate)
{
  if(!CheckCollision(index, direction))
  switch (direction)
  {
    case 0:
      characters[index].posY--;
      break;
    case 1:
      characters[index].posY++;
      break;
    case 2:
      characters[index].posX--;
      break;
    case 3:
      characters[index].posX++;
      break;
    default:
      break;
  }
  
  if (characters[index].posX < 0)
    characters[index].posX = mapWidth - 1;
  if (characters[index].posY < 0)
    characters[index].posY = mapHeight - 1;
  
  if (characters[index].posX >= mapWidth)
    characters[index].posX = 0;
  if (characters[index].posY >= mapHeight)
    characters[index].posY = 0;
  
  if (cameraUpdate)
    CameraFollow(index);
}

int CheckInput()
{
    if (InputUp()) {MoveCharacter(0, 0, true); return 1;}
    if (InputLeft()) {MoveCharacter(0, 2, true); return 1;}
    if (InputRight()) {MoveCharacter(0, 3, true); return 1;}
    if (InputDown()) {MoveCharacter(0, 1, true); return 1;}
  return 0;
}
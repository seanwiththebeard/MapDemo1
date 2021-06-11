#include <peekpoke.h>
#include "System_Graphics.h"
#include "common.h"
#include "System_Input.h"
#include "System_MessageWindow.h"
//#include "cbm_petscii_charmap.h"

//Map Data (set these all to the same number)
int MapData = 0xB000;
int mapHeight = 16;
int mapWidth = 16;
byte mapData[16][16];
bool charsDrawn[16][16];
bool DrawThisFrame = true;

//Viewport
byte viewportPosX = 0;
byte viewportPosY = 0;
byte viewportWidth = 11;
byte viewportHeight = 11;

int viewportOrigin = 0xC800;
int colorOrigin = 0xD800;

int yCols[25];

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
  byte trigger;
} tiles[16];

void DrawTile(byte index, byte xpos, byte ypos)
{
  int tileAddress, colorAddress;
  tileAddress = viewportOrigin + xpos + yCols[ypos];
  colorAddress = tileAddress + 0x1000;
          
  POKEW(tileAddress, PEEKW(&tiles[index].chars[0]));
  POKEW(tileAddress + COLS, PEEKW(&tiles[index].chars[2]));
  POKEW(colorAddress, PEEKW(&tiles[index].colors[0]));
  POKEW(colorAddress + COLS, PEEKW(&tiles[index].colors[2]));
}

struct Character
{
  byte chars[4];
  byte colors[4];
  byte trigger;
  byte combat;
  int posX;
  int posY;
  bool visible;
} characters[16];

void DrawCharacter(byte index, int xpos, int ypos)
{
  {
    int tileAddress, colorAddress;
    tileAddress = viewportOrigin + xpos + yCols[ypos];
    colorAddress = tileAddress + 0x1000;
          
    POKEW(tileAddress, PEEKW(&characters[index].chars[0]));
    POKEW(tileAddress + COLS, PEEKW(&characters[index].chars[2]));
    POKEW(colorAddress, PEEKW(&characters[index].colors[0]));
    POKEW(colorAddress + COLS, PEEKW(&characters[index].colors[2]));
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
  byte yOffset = 0;
  
  viewportOrigin += (viewportPosX + COLS * viewportPosY);
  colorOrigin += (viewportPosX + COLS * viewportPosY);
  
  cameraOffsetX = viewportWidth / 2;
  cameraOffsetY = viewportHeight / 2;
  
  BlankCharsDrawn();
  
  //Init Tileset
  //Water
  tiles[0].chars[0] = 132;
  tiles[0].chars[1] = 133;
  tiles[0].chars[2] = 134;
  tiles[0].chars[3] = 135;
  tiles[0].colors[0] = 14;
  tiles[0].colors[1] = 14;
  tiles[0].colors[2] = 14;
  tiles[0].colors[3] = 14;
  
  //Signpost
  tiles[1].chars[0] = 148;
  tiles[1].chars[1] = 149;
  tiles[1].chars[2] = 150;
  tiles[1].chars[3] = 151;
  tiles[1].colors[0] = 8;
  tiles[1].colors[1] = 8;
  tiles[1].colors[2] = 9;
  tiles[1].colors[3] = 9;
  tiles[1].blocked = 255;
  //WriteBit(&tiles[84].blocked, 0, true);
  //WriteBit(&tiles[84].blocked, 2, true);  
  
  //Grass
  tiles[2].chars[0] = 136;
  tiles[2].chars[1] = 137;
  tiles[2].chars[2] = 138;
  tiles[2].chars[3] = 139;
  tiles[2].colors[0] = 7;
  tiles[2].colors[1] = 13;
  tiles[2].colors[2] = 13;
  tiles[2].colors[3] = 7;
  
  //Init Characters
  for (i = 0; i < 16; i++)
  {
    characters[i].chars[0] = 8 + i * 16;
    characters[i].chars[1] = 9 + i * 16;
    characters[i].chars[2] = 10 + i * 16;
    characters[i].chars[3] = 11 + i * 16;
    characters[i].colors[0] = i + 1;
    characters[i].colors[1] = i + 1;
    characters[i].colors[2] = i + 1;
    characters[i].colors[3] = i + 1;
    characters[i].posX = i + 4;
    characters[i].posY = i + 4;
    characters[i].visible = false;
  }
    characters[0].visible = true;
    characters[0].posX += 2;
  
  for (i = 0; i < 25; i++)
    yCols[i] = i*COLS;
  
  //Init map data
  for(y = 0; y < mapHeight; y++)
    {
      for(x = 0; x < mapWidth; x++)
      {
        mapData[x][y] = 0;
      }
    yOffset+= COLS;
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

int DrawMap()
{
  if(!DrawThisFrame)
    return 0;

  PrintString("Drawing         ", 0, 24, true);

  CameraFollow(0);
  BlankCharsDrawn();
  
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
      //bool charDrawn = false;
      
      //Wrap the map data X reference
      if (a == mapWidth)
          a = 0;
      if (a < 0)
          a += mapWidth;
      
      if (!charsDrawn[x][y])
        DrawTile(mapData[a][b], x*2, y*2);
      
      a++;
    }
    
    a = offsetX;
    b++;
  }
    PrintString("                ", 0, 24, true);
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

void MapUpdate()
{
  DrawThisFrame = true;
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
  else
    if(index == 0)
    {
      //DrawThisFrame = false;
      WriteLineMessageWindow("Collision!@");
      FlashColor(2, 10);
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
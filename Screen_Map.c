#include <peekpoke.h>
#include "System_Graphics.h"
#include "common.h"
#include "System_Input.h"
#include "System_MessageWindow.h"
#include "cbm_petscii_charmap.h"
#include "System_CharacterSets.h"

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
byte viewportBuffer[11][11];
byte DoubleBufferChars[(11*2)*(11*2)];
byte DoubleBufferColors[(11*2)*(11*2)];

byte followIndex = 0;

byte NextLineOffset;


int viewportOrigin = 0xC800;
int colorOrigin = 0xD800;

int tileAddress, colorAddress, tileAddressOdd, colorAddressOdd;
int offsetViewportChar, offsetViewportColor, offsetViewportCharOdd, offsetViewportColorOdd;

int yCols[25];

//Camera Position
int offsetX, offsetY = 0;
//int x, y, a, b, i = 0;
byte cameraOffsetX = 0;
byte cameraOffsetY = 0;

//Color Palette
byte ColorPalette[256];

byte moved = 0;

//Tile Data
struct Tile
{
  byte chars[4];
  byte index;
  byte colors[4];
  byte blocked;
  byte trigger;
} tiles[64];

void DrawTile(byte index, byte xpos, byte ypos)
{
  tileAddress = viewportOrigin + xpos * 2 + yCols[ypos] * 2;
  colorAddress = tileAddress + 0x1000;
          
  POKEW(tileAddress, PEEKW(&tiles[index].chars[0]));
  POKEW(tileAddress + COLS, PEEKW(&tiles[index].chars[2]));
  POKEW(colorAddress, PEEKW(&tiles[index].colors[0]));
  POKEW(colorAddress + COLS, PEEKW(&tiles[index].colors[2]));
}

void DrawBufferTile(byte index, byte xpos, byte ypos)
{
  int bufferAddress = (int) &DoubleBufferChars[xpos * 2 + ypos*viewportWidth * 2];
  int colorBufferAddress = (int) &DoubleBufferColors[xpos * 2 + ypos*viewportWidth * 2];

  POKEW(bufferAddress, PEEKW(&tiles[index].chars[0]));
  POKEW(bufferAddress + viewportWidth * 2, PEEKW(&tiles[index].chars[2]));
  POKEW(colorBufferAddress, PEEKW(&tiles[index].colors[0]));
  POKEW(colorBufferAddress + viewportWidth * 2, PEEKW(&tiles[index].colors[2]));
  }

void DrawTileTop(byte index)
{
  CopyMemory((int) &DoubleBufferChars[offsetViewportChar], (int) &tiles[index].chars[0], 2);
  CopyMemory((int) &DoubleBufferColors[offsetViewportColor], (int) &tiles[index].colors[0], 2);

  //CopyMemory(tileAddress, &tiles[index].chars[0], 2);
  //CopyMemory(colorAddress, &tiles[index].colors[0], 2);
  
  //POKEW(tileAddress, PEEKW(&tiles[index].chars[0]));
  //POKEW(colorAddress, PEEKW(&tiles[index].colors[0]));
}

void DrawTileBottom(byte index)
{
  CopyMemory((int) &DoubleBufferChars[offsetViewportCharOdd], (int) &tiles[index].chars[2], 2);
  CopyMemory((int) &DoubleBufferColors[offsetViewportColorOdd], (int) &tiles[index].colors[2], 2);

  //CopyMemory(tileAddressOdd, &tiles[index].chars[2], 2);
  //CopyMemory(colorAddressOdd, &tiles[index].colors[2], 2);

  //POKEW(tileAddressOdd, PEEKW(&tiles[index].chars[2]));
  //POKEW(colorAddressOdd, PEEKW(&tiles[index].colors[2]));
}

struct Character
{
  byte tile;
  byte chars[4];
  byte colors[4];
  byte trigger;
  byte combat;
  int posX;
  int posY;
  bool visible;
  bool collide;
  byte message;
} characters[16];

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

void CameraFollow()
{  
  int x, y;
  offsetX = characters[followIndex].posX;
  offsetY = characters[followIndex].posY;
  
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

int GetWrappedX(int xPos) //For viewport character positions
{ 
  int tempX = xPos - offsetX;
  
  if (xPos < offsetX)
    tempX += mapWidth;

  return tempX;
}

int GetWrappedY(int YPos)
{
  int tempY = YPos - offsetY;
  
  if (YPos < offsetY)
    tempY += mapHeight;

  return tempY;
}

void DrawChar(byte index)
{
  byte posx = GetWrappedX(characters[index].posX);
  if (posx < viewportWidth)
  {
    byte posy = GetWrappedY(characters[index].posY);
    if (posy < viewportHeight)
      if(characters[index].visible)
      {
        //viewportBuffer[posx][posy] = characters[index].tile;
        DrawTile(index, posx, posy);
        //charsDrawn[posx][posy] = true;
      }
  }
}

void BufferCharacters()
{
  int i;
  for(i = 0; i < 16; i++)
    DrawChar(i);
}

void UpdateViewport()
{
  int x, offset;
  for (x = 0; x < viewportHeight * 2; x++)
  {
    offset = x * COLS;
    CopyMemory((int) (viewportOrigin + offset), (int) &DoubleBufferChars[x * viewportWidth * 2], viewportWidth*2);
    CopyMemory((int) (colorOrigin + offset), (int) &DoubleBufferColors[x * viewportWidth * 2], viewportWidth*2);
  }

  BufferCharacters();
}

void DrawViewport()
{
  int x, y;

  tileAddress = viewportOrigin + yCols[0 << 1];
  colorAddress = tileAddress + 0x1000;
  tileAddressOdd = tileAddress + COLS;
  colorAddressOdd = colorAddress + COLS;

  offsetViewportChar = 0;
  offsetViewportColor = 0;
  offsetViewportCharOdd = viewportWidth * 2;
  offsetViewportColorOdd = viewportWidth * 2;

  for (y = 0; y < viewportHeight; y++)
  {
    for (x = 0; x < viewportWidth; x++)
    {
      DrawTileTop(viewportBuffer[x][y]);
      DrawTileBottom(viewportBuffer[x][y]);

      offsetViewportChar += 2;
      offsetViewportColor += 2;
      offsetViewportCharOdd += 2;
      offsetViewportColorOdd += 2;
    }
    offsetViewportChar += 2 * viewportWidth;
    offsetViewportColor += 2 * viewportWidth;
    offsetViewportCharOdd += 2 * viewportWidth;
    offsetViewportColorOdd += 2 * viewportWidth;
  }

  /*for (y = 0; y < viewportHeight; y++)
  {
    tileAddress = viewportOrigin + COLS + yCols[y << 1];
    colorAddress = tileAddress + 0x1000;
    for (x = 0; x < viewportWidth; x++)
    {
      tileAddress += 2;
      colorAddress += 2;
    }
  }*/
  UpdateViewport();
}

void DrawSingleRow(int row)
{
  int a, b, x, y;
  a = offsetX;
  b = offsetY;
  
  y = row;
  {


    for(x = 0; x < viewportWidth; x++)
    {      
      //Wrap the map data X reference
      if (a == mapWidth)
          a = 0;
      if (a < 0)
          a += mapWidth;
      {
        viewportBuffer[x][y] = mapData[a][b];
        DrawBufferTile(viewportBuffer[x][y], x, y);
      }
      a++;
    }

  }

  UpdateViewport();
}

void ScrollViewport(byte direction)
{  
  int x = 0;
  int totalSize = 2*viewportHeight * 2*viewportWidth;

  ClampOffset();

  switch (direction)
  {
    case 0:
      {
        for (x = 2*viewportHeight * 2*viewportWidth - 1 - viewportWidth * 4; x > 0; x -= viewportWidth * 4)
        {
          CopyMemory((int) &DoubleBufferChars[x], (int) &DoubleBufferChars[x - viewportWidth * 4], viewportWidth*4);
          CopyMemory((int) &DoubleBufferColors[x], (int) &DoubleBufferColors[x - viewportWidth * 4], viewportWidth*4);
        }
        DrawSingleRow(0);
      }
    break;
    case 1:
    break;
    case 2:
    break;
    case 3:
    break;
    default:
    break;
  }
}

void InitializeMapData()
{
  int x, y, i;

  byte grass = 36;
  byte water = 34;
  byte signpost = 35;
  byte yOffset = 0;
  
  NextLineOffset = (COLS - (viewportWidth << 1)) + COLS;
  viewportOrigin += (viewportPosX + COLS * viewportPosY);
  colorOrigin += (viewportPosX + COLS * viewportPosY);
  
  cameraOffsetX = viewportWidth / 2;
  cameraOffsetY = viewportHeight / 2;
  
  for (y = 0; y < 8; y++)
    for (x = 0; x < 8; x++)
    {
      byte index = x + y*8;
      byte offset = x * 2 + 32*y;

      tiles[index].index = index;
      tiles[index].chars[0] = offset;
      tiles[index].chars[1] = offset + 1;
      tiles[index].chars[2] = offset + 16;
      tiles[index].chars[3] = offset + 17;

      tiles[index].colors[0] = AttributeSet[0][offset];
      tiles[index].colors[1] = AttributeSet[0][offset + 1];
      tiles[index].colors[2] = AttributeSet[0][offset + 16];
      tiles[index].colors[3] = AttributeSet[0][offset + 17];
    }

  //Init Tileset
  //Water
  
  //Signpost
  //tiles[signpost].blocked = 255;
  WriteBit(&tiles[signpost].blocked, 0, true);
  WriteBit(&tiles[signpost].blocked, 2, true);  
  
  //Grass
  
  //Init Characters
  for (i = 0; i < 16; i++)
  {
    characters[i].tile = i;
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
    characters[i].collide = false;
  }
    characters[0].visible = true;
    characters[0].posX += 2;
    characters[0].tile = 2;

    characters[1].visible = true;
    characters[1].collide = true;
    characters[1].message = 0;

    characters[2].tile = signpost;
    characters[2].visible = true;
    characters[2].collide = true;
    characters[2].message = 1;
  
  for (i = 0; i < 25; i++)
    yCols[i] = i*COLS;
  
  //Init map data
  for(y = 0; y < mapHeight; y++)
    {
      for(x = 0; x < mapWidth; x++)
      {
        mapData[x][y] = water;
      }
    yOffset+= COLS;
    }  
  mapData[4][4] = grass;
  mapData[5][4] = grass;
  mapData[6][4] = grass;
  mapData[7][4] = grass;
  mapData[4][5] = grass;
  mapData[5][5] = grass;
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

int DrawMap()
{
  int a, b, x, y;
  if(!DrawThisFrame)
    return 0;

  //wait_vblank(1);
  //SetBackground(6);

  CameraFollow();
  //BlankCharsDrawn();
  
  ClampOffset();
  
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
      {
        viewportBuffer[x][y] = mapData[a][b];
      }
      a++;
    }
    a = offsetX;
    b++;
  }
  DrawViewport();
}

int wrapX(int posX) //Used in map positions
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
  int i;

  int xPos = characters[charIndex].posX;
  int yPos = characters[charIndex].posY;
  
  if(ReadBit(tiles[mapData[xPos][yPos]].blocked, Direction))
    return true;
  
  switch (Direction)
  {
    case 0:
      //yPos -= 1;
      yPos = wrapY(yPos - 1);
      Direction = 1;
      break;
    case 1:
      //yPos += 1;
      yPos = wrapY(yPos + 1);
      Direction = 0;
      break;
    case 2:
      //xPos -= 1;
      xPos = wrapX(xPos) - 1;
      Direction = 3;
      break;
    case 3:
      //xPos += 1;
      xPos = wrapX(xPos + 1);
      Direction = 2;
      break;
    default:
      return false;
  }
  
  if(ReadBit(tiles[mapData[xPos][yPos]].blocked, Direction))
    return true;
  for (i = 0; i < 16; i++)
    if(characters[i].collide)
      if (characters[i].posX == xPos)
        if (characters[i].posY == yPos)
          {
            WriteLineMessageWindow(Messages[characters[i].message], 0);
            return true;
          }
  
  return false;
}

void MoveCharacter(byte index, byte direction, bool cameraUpdate)
{
  bool checkCollision = CheckCollision(index, direction);
  if(!checkCollision)
    {
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
        CameraFollow();
      if (index == followIndex)
        ScrollViewport(direction);
  }
  else
    if(index == 0)
    {
      DrawThisFrame = false;
      //FlashColor(2, 1);
      //WriteLineMessageWindow("Collision!@", 0);
    }
}

int CheckInput()
{
    if (InputUp())
    {
      MoveCharacter(0, 0, true);
      //DrawMap();
      return 1;
    }
    if (InputDown()) 
    {
      MoveCharacter(0, 1, true); 
      DrawMap(); 
      return 1;
    }
    if (InputLeft()) {MoveCharacter(0, 2, true); DrawMap(); return 1;}
    if (InputRight()) {MoveCharacter(0, 3, true); DrawMap(); return 1;}
  return 0;
}
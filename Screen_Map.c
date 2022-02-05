#include <stdio.h>
#include <peekpoke.h>
#include "System_Graphics.h"
#include "Common.h"
#include "System_Input.h"
#include "System_MessageWindow.h"
#include "System_CharacterSets.h"

//Map Data
#define mapHeight 64
#define mapWidth 64
byte mapData[mapWidth][mapHeight];
bool DrawThisFrame = true;
bool wrap = true;

//Viewport
#define viewportPosX 0
#define viewportPosY 0
#define viewportWidth 11
#define viewportHeight 11
#define viewportCharWidth (viewportWidth * 2)
#define viewportCharHeight (viewportHeight * 2)
#define viewportWidthQuad (viewportWidth*4)
#define charactersCount 16
#define LastMapScanline (8*viewportPosY + 16*viewportHeight)
byte viewportBuffer[viewportWidth][viewportHeight];
byte DoubleBufferChars[viewportCharWidth*viewportCharHeight];
byte DoubleBufferColors[viewportCharWidth*viewportCharHeight];

byte followIndex = 0;

int viewportOrigin = (int)&ScreenDoubleBuffer[0][0];
int colorOrigin = (int)&ScreenDoubleBuffer[1][0];

int tileAddress, colorAddress, tileAddressOdd, colorAddressOdd;
int offsetViewportChar, offsetViewportColor, offsetViewportCharOdd, offsetViewportColorOdd;


//Camera Position
int offsetX, offsetY = 0;
byte cameraOffsetX = 0;
byte cameraOffsetY = 0;

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

struct
{
    int CharIndex[4];
    byte Chars[2];
    byte ScatterIndex;
    byte NPCIndex;
    byte MusicIndex;
}ScreenQuad[256]={
    {
        {130, 131, 146, 147},
      	{32, 33},
        0, 0, 0
    },
    {
        {0, 0, 0, 0},
      	{0, 1},
        0, 0, 0
    } 
};

void DrawTile(byte index, byte xpos, byte ypos)
{
  tileAddress = viewportOrigin + xpos * 2 + YColumnIndex[ypos] * 2;
  colorAddress = tileAddress + 1024;
          
  POKEW(tileAddress, PEEKW(&tiles[index].chars[0]));
  POKEW(tileAddress + COLS, PEEKW(&tiles[index].chars[2]));
  POKEW(colorAddress, PEEKW(&tiles[index].colors[0]));
  POKEW(colorAddress + COLS, PEEKW(&tiles[index].colors[2]));
}

void DrawBufferTile(byte index, byte xpos, byte ypos)
{
  int offset = xpos * 2 + ypos*(viewportWidthQuad);
  int bufferAddress = (int) &DoubleBufferChars[offset];
  int colorBufferAddress = (int) &DoubleBufferColors[offset];

  CopyMemory((int)bufferAddress, (int) &tiles[index].chars[0], 2);
  CopyMemory((int)colorBufferAddress, (int) &tiles[index].colors[0], 2);
  
  bufferAddress += viewportCharWidth;
  colorBufferAddress += viewportCharWidth;
  CopyMemory((int)bufferAddress, (int) &tiles[index].chars[2], 2);
  CopyMemory((int)colorBufferAddress, (int) &tiles[index].colors[2], 2);

  //POKEW(bufferAddress, PEEKW(&tiles[index].chars[0]));
  //POKEW(bufferAddress + viewportWidth * 2, PEEKW(&tiles[index].chars[2]));
  //POKEW(colorBufferAddress, PEEKW(&tiles[index].colors[0]));
  //POKEW(colorBufferAddress + viewportWidth * 2, PEEKW(&tiles[index].colors[2]));
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
} characters[charactersCount];

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
  
    for(x = 0; x < cameraOffsetX; ++x)
    {
      --offsetX;
      ClampOffset();
    }
  
  for(y = 0; y < cameraOffsetY; ++y)
    {
      --offsetY;
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
        DrawTile(characters[index].tile, posx, posy);
        //charsDrawn[posx][posy] = true;
      }
  }
}

void BufferCharacters()
{
  int i;
  for(i = 0; i < charactersCount; ++i)
    DrawChar(i);
}

void UpdateViewport()
{
  int x, offset;
  for (x = 0; x < viewportCharHeight; ++x)
  {
    int index = x * viewportCharWidth;
    offset = YColumnIndex[x];
    CopyMemory((int) (viewportOrigin + offset), (int) &DoubleBufferChars[index], viewportCharWidth);
    CopyMemory((int) (colorOrigin + offset), (int) &DoubleBufferColors[index], viewportCharWidth);
  }
  BufferCharacters();
  //CopyDoubleBuffer();
  //raster_wait(255);
  //CopyDoubleBuffer();
  CopyDoubleBufferArea(viewportPosX, viewportPosY, viewportCharWidth, viewportCharHeight);
}

void DrawSingleRow(int row)
{
  int a, b, x, y;

  CameraFollow();  
  //ClampOffset();
  
  a = offsetX;
  b = offsetY;
  
  for(y = 0; y < viewportHeight; ++y)
  {
    //Wrap the map data y reference
    if (b == mapHeight)
        b = 0;
    if (b < 0)
      b +=mapHeight;
    if (y == row)
    {
      for(x = 0; x < viewportWidth; ++x)
      {
        //Wrap the map data X reference
        if (a == mapWidth)
          a = 0;
        if (a < 0)
          a += mapWidth;
        
        viewportBuffer[x][y] = mapData[a][b];
        DrawBufferTile(viewportBuffer[x][y], x, y);
      a++;
      }
      break;
    }
    a = offsetX;
    ++b;
  }
  UpdateViewport();
}

void DrawSingleColumn(int column)
{
  int a, b, x, y;
  CameraFollow();
  
  a = offsetX;
  b = offsetY;
  
  for(y = 0; y < viewportHeight; ++y)
  {
    //Wrap the map data y reference
    if (b == mapHeight)
        b = 0;
    if (b < 0)
      b +=mapHeight;
    {
      for(x = 0; x < viewportWidth; ++x)
      {
        //Wrap the map data X reference
        if (a == mapWidth)
          a = 0;
        if (a < 0)
          a += mapWidth;
        if (x == column)
        {
          viewportBuffer[x][y] = mapData[a][b];
          DrawBufferTile(viewportBuffer[x][y], x, y);
        }
      ++a;
      }
    }
    a = offsetX;
    ++b;
  }
  UpdateViewport();
}

void LoadQuadrant(byte index, byte quad)
{
  #define quadWidth 8
  #define quadHeight 8
  byte x, y;
  byte originX = 0;
  byte originY = 0;
  int chardata;

  switch (quad)
  {
    case 0:
    break;
    case 1:
    originX = quadWidth;
    break;
    case 2:
    originY = quadHeight;
    break;
    case 3:
    originX = quadWidth;
    originY = quadHeight;
    break;
    default:
    WriteLineMessageWindow("bad quad", 0);
    break;
  }

  chardata = CharacterRAM + 8*ScreenQuad[index].CharIndex[quad];
  for (y = 0; y < 8; ++y)
  {
    for (x = 0; x < 8; ++x)
    {
      if (ReadBit(PEEK(chardata + y), 7 - x) > 0)
      {
        mapData[x + originX][y + originY] = ScreenQuad[index].Chars[1];
      }
      else
      {
        mapData[x + originX][y + originY] = ScreenQuad[index].Chars[0];
      }
    }
  }
}

void InitializeMapData()
{
  int x, y, i;

  #define grass 36
  #define water 34
  #define signpost 35
  byte yOffset = 0;
  
  viewportOrigin += (viewportPosX + COLS * viewportPosY);
  colorOrigin += (viewportPosX + COLS * viewportPosY);
  
  cameraOffsetX = viewportWidth / 2;
  cameraOffsetY = viewportHeight / 2;
  
  for (y = 0; y < 8; ++y)
    for (x = 0; x < 8; ++x)
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
      
      tiles[index].blocked = 0;
    }

  //Init Tileset
  //Water
  
  //Signpost
  //tiles[signpost].blocked = 255;
  //WriteBit(&tiles[signpost].blocked, 0, true);
  //WriteBit(&tiles[signpost].blocked, 2, true);  
  
  //Grass
  
  //Init Characters
  for (i = 0; i < 16; ++i)
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
  
  //Init map data
  for(y = 0; y < mapHeight; ++y)
    {
      for(x = 0; x < mapWidth; ++x)
      {
        mapData[x][y] = water;
      }
    yOffset+= COLS;
    }  

  LoadQuadrant(0, 0);
  LoadQuadrant(0, 1);
  LoadQuadrant(0, 2);
  LoadQuadrant(0, 3);
}

void ScrollViewport(byte direction)
{  
  int x, y;
  int totalSize = 2*viewportHeight * 2*viewportWidth;

  CameraFollow();
  //ClampOffset();

  switch (direction)
  {
    case 0:
      {
        for (x = totalSize - viewportWidth * 4; x > 0; x -= viewportWidth * 4)
        {
          CopyMemory((int) &DoubleBufferChars[x], (int) &DoubleBufferChars[x - viewportWidth * 4], viewportWidthQuad);
          CopyMemory((int) &DoubleBufferColors[x], (int) &DoubleBufferColors[x - viewportWidth * 4], viewportWidthQuad);
        }
        DrawSingleRow(0);
      }
      break;
    case 1:
    {
      //for (x = 0; x < totalSize - viewportWidthQuad; ++x)
      {
      CopyMemory((int) &DoubleBufferChars[0], (int) &DoubleBufferChars[viewportWidth * 4], totalSize - viewportWidthQuad);
      CopyMemory((int) &DoubleBufferColors[0], (int) &DoubleBufferColors[viewportWidth * 4], totalSize - viewportWidthQuad);
      }
      DrawSingleRow(viewportHeight - 1);
    }
    break;
    case 2:
    {
      int length = viewportCharWidth - 2;
      byte buffer[40];

      for (y = 0; y < viewportCharHeight; ++y)
      {
        int offset = y*viewportCharWidth;
        CopyMemory((int)&buffer[0], (int) &DoubleBufferChars[offset], length);
        CopyMemory((int)&DoubleBufferChars[offset + 2], (int) &buffer[0], length);
        CopyMemory((int)&buffer[0], (int) &DoubleBufferColors[offset], length);
        CopyMemory((int)&DoubleBufferColors[offset + 2], (int) &buffer[0], length);
      }
      DrawSingleColumn(0);
    }
    break;
    case 3:
    {
      int length = viewportCharWidth - 2;
      byte buffer[40];

      for (y = 0; y < viewportCharHeight; ++y)
      {
        int offset = y*viewportCharWidth;
        CopyMemory((int)&buffer[0], (int) &DoubleBufferChars[offset + 2], length);
        CopyMemory((int)&DoubleBufferChars[offset], (int) &buffer[0], length);
        CopyMemory((int)&buffer[0], (int) &DoubleBufferColors[offset + 2], length);
        CopyMemory((int)&DoubleBufferColors[offset], (int) &buffer[0], length);
      }
      DrawSingleColumn(viewportWidth - 1);
    }
    break;
    default:
    break;
  }
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
  for (i = 0; i < charactersCount; ++i)
    if(characters[i].collide)
      if (characters[i].posX == xPos)
        if (characters[i].posY == yPos)
          {
            WriteLineMessageWindow(Messages[characters[i].message], 1);
            return true;
          }
  
  return false;
}

void DrawEntireMap()
{
  int x, y, a, b, c, index;
  ScreenDisable();

  CameraFollow();

  a = offsetX;
  b = offsetY;

  //Buffer the matrix of tiles for our viewport
  for(y = 0; y < viewportHeight; ++y)
  {
    //Wrap the map data y reference
    if (b == mapHeight)
        b = 0;
    if (b < 0)
      b +=mapHeight;
      
    for(x = 0; x < viewportWidth; ++x)
      {
        //Wrap the map data X reference
        if (a == mapWidth)
          a = 0;
        if (a < 0)
          a += mapWidth;
        
        index = mapData[a][b];
        viewportBuffer[x][y] = index;
        a++;
      }
    a = offsetX;
    ++b;
  }

  //Insert Characters
  {
    for (c = 0; c < charactersCount; ++c)
    {
      //if(characters[index].visible)
      {
        byte posx = GetWrappedX(characters[c].posX);
        if (posx < viewportWidth)
        {
          byte posy = GetWrappedY(characters[c].posY);
          if (posy < viewportHeight)
            viewportBuffer[posx][posy] = characters[c].tile;
        }
      }
    }
  }

  //Buffer the viewport
  {
    for (y = 0; y < viewportHeight; y++)
      for (x = 0; x < viewportWidth; x++)
        DrawBufferTile(viewportBuffer[x][y], x, y);
  }

  //Update the viewport
  {
    int offset;
    //wait_vblank(1);

    for (x = 0; x < viewportCharHeight; ++x)
    {
      offset = YColumnIndex[x];
      CopyMemory((int) (viewportOrigin + offset), (int) &DoubleBufferChars[x * viewportCharWidth], viewportCharWidth);
      CopyMemory((int) (colorOrigin + offset), (int) &DoubleBufferColors[x * viewportCharWidth], viewportCharWidth);
    }
      CopyDoubleBufferArea(viewportPosX, viewportPosY, viewportCharWidth, viewportCharHeight);
  }
  ScreenEnable();
}

void MoveCharacter(byte index, byte direction, bool cameraUpdate)
{
  bool checkCollision = CheckCollision(index, direction);
  if(!checkCollision)
    {
      switch (direction)
      {
        case 0:
          --characters[index].posY;
          if (characters[index].posY < 0)
            characters[index].posY = mapHeight - 1;
          break;
        case 1:
           ++characters[index].posY;
          if (characters[index].posY >= mapHeight)
            characters[index].posY = 0;
          break;
        case 2:
           --characters[index].posX;
           if (characters[index].posX < 0)
            characters[index].posX = mapWidth - 1;
           break;
        case 3:
           ++characters[index].posX;
           if (characters[index].posX >= mapWidth)
            characters[index].posX = 0;
          break;
        default:
          break;
      }

      if (cameraUpdate)
        CameraFollow();
      if (index == followIndex)
        //DrawEntireMap();
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
      return 1;
    }
    if (InputDown()) 
    {
      MoveCharacter(0, 1, true); 
      return 1;
    }
    if (InputLeft())
    {
      MoveCharacter(0, 2, true);
      return 1;
    }
    if (InputRight())
    {
      MoveCharacter(0, 3, true);
      return 1;
    }
    if (InputFire())
    {
      byte string[16];
      sprintf(string, "Pos = %d,%d@", characters[0].posX, characters[0].posY);
      DrawEntireMap();
      WriteLineMessageWindow(string, 1);
      return 1;
    }
  return 0;
}
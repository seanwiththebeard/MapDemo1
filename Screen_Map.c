#include <stdio.h>
#include <peekpoke.h>
#include "System_Graphics.h"
#include "Common.h"
#include "System_Input.h"
#include "System_MessageWindow.h"
#include "System_CharacterSets.h"

//Map Data
#define mapHeight 32
#define mapWidth 32
byte mapData[mapWidth][mapHeight];
bool wrap = true;

//Viewport
#define viewportPosX 0
#define viewportPosY 0
#define viewportWidth 13
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
}ScreenQuad[64];

void DrawTile(byte index, byte xpos, byte ypos) //Draws directly to the graphics double-buffer
{
  int offset = xpos * 2 + YColumnIndex[ypos] * 2;
  int tileAddress = viewportOrigin + offset;
  int colorAddress = colorOrigin + offset;
          
  POKEW(tileAddress, PEEKW(&tiles[index].chars[0]));
  POKEW(tileAddress + COLS, PEEKW(&tiles[index].chars[2]));
  POKEW(colorAddress, PEEKW(&tiles[index].colors[0]));
  POKEW(colorAddress + COLS, PEEKW(&tiles[index].colors[2]));
}

void DrawBufferTile(byte index, byte xpos, byte ypos) //Draws into the map viewport double-buffer
{
  int offset = xpos * 2 + ypos*(viewportWidthQuad);
  int tileAddress = (int) &DoubleBufferChars[offset];
  int colorAddress = (int) &DoubleBufferColors[offset];

  POKEW(tileAddress, PEEKW(&tiles[index].chars[0]));
  POKEW(tileAddress + viewportCharWidth, PEEKW(&tiles[index].chars[2]));
  POKEW(colorAddress, PEEKW(&tiles[index].colors[0]));
  POKEW(colorAddress + viewportCharWidth, PEEKW(&tiles[index].colors[2]));
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

#define ClampOffsetX(){if(offsetX >= mapWidth)offsetX = 0; if (offsetX < 0)offsetX = mapWidth - 1;}
#define ClampOffsetY(){if(offsetY >= mapHeight)offsetY = 0; if (offsetY < 0) offsetY = mapHeight - 1;}

void CameraFollow()
{  
  int x, y;
  offsetX = characters[followIndex].posX;
  offsetY = characters[followIndex].posY;
  
    for(x = 0; x < cameraOffsetX; ++x)
    {
      --offsetX;
      ClampOffsetX();
    }
  
  for(y = 0; y < cameraOffsetY; ++y)
    {
      --offsetY;
      ClampOffsetY();
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

int WrapMapPositionX(int posX)
{  
  if (posX < 0)
  {
    return (mapWidth - 1);
  }
  if (posX == mapWidth)
  {
    return 0;
  }
  return posX;
}

int WrapMapPositionY(int posY)
{  
  if (posY < 0)
  {
    return (mapHeight - 1);
  }
  if (posY == mapHeight)
  {
    return 0;
  }
  return posY;
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

void UpdateViewport() //Copies the viewport buffer to the screen buffer
{
  int x, offset, index;
  for (x = 0; x < viewportCharHeight; ++x)
  {
    index = x * viewportCharWidth;
    offset = YColumnIndex[x];
    
    CopyMemory((int)(viewportOrigin + offset), (int) &DoubleBufferChars[index], viewportCharWidth);
    CopyMemory((int)(colorOrigin + offset), (int) &DoubleBufferColors[index], viewportCharWidth);    
  }
  BufferCharacters();
  //CopyDoubleBuffer();
  //raster_wait(255);
  //CopyDoubleBuffer();
  //CopyDoubleBufferRows(viewportPosY, viewportCharHeight, viewportCharWidth);
  CopyDoubleBufferArea(viewportPosX, viewportPosY, viewportCharWidth, viewportCharHeight);
}

void DrawSingleRow(int row)
{
  int a, b, x, y;

  CameraFollow();
  
  a = offsetX;
  b = offsetY;
  
  for(y = 0; y < viewportHeight; ++y)
  {    
    b = WrapMapPositionY(b);
    if (y == row)
    {
      for(x = 0; x < viewportWidth; ++x)
      {
        a=WrapMapPositionX(a);
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
    b = WrapMapPositionY(b);
    {
      for(x = 0; x < viewportWidth; ++x)
      {
        a = WrapMapPositionX(a);
        viewportBuffer[x][y] = mapData[a][b];
        
        if (x == column)
          DrawBufferTile(viewportBuffer[x][y], x, y);
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
  byte x, y, z;
  byte originX = 0;
  byte originY = 0;
  int chardata;

  for (z = 0; z < 4; ++z)
  {
    switch (quad)
    {
    case 0:
      originX = 0;
      originY = 0;
      break;
    case 1:
      originX = quadWidth * 2;
      originY = 0;
      break;
    case 2:
      originX = 0;
      originY = quadHeight * 2;
      break;
    case 3:
      originX = quadWidth * 2;
      originY = quadHeight * 2;
      break;
    default:
      break;
    }

    switch (z)
    {
    case 0:
      break;
    case 1:
      originX += quadWidth;
      break;
    case 2:
      originY += quadHeight;
      break;
    case 3:
      originX += quadWidth;
      originY += quadHeight;
      break;
    default:
      break;
    }

    chardata = CharacterRAM + 8*ScreenQuad[index].CharIndex[z];
    for (y = 0; y < quadHeight; ++y)
    {
      for (x = 0; x < quadWidth; ++x)
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


      ScreenQuad[index].CharIndex[0] = offset;
      ScreenQuad[index].CharIndex[1] = offset + 1;
      ScreenQuad[index].CharIndex[2] = offset + 16;
      ScreenQuad[index].CharIndex[3] = offset + 17;
      ScreenQuad[index].Chars[0] = 32;
      ScreenQuad[index].Chars[1] = index;
      ScreenQuad[index].MusicIndex = 0;
      ScreenQuad[index].NPCIndex = 0;
      ScreenQuad[index].ScatterIndex = 0;
    }

    ScreenQuad[2].Chars[0] = 36;
    ScreenQuad[2].Chars[1] = 44;

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
    characters[2].message = 2;
    characters[2].posX = 8;
    characters[2].posY = 23;

  
  //Init map data
  for(y = 0; y < mapHeight; ++y)
    {
      for(x = 0; x < mapWidth; ++x)
      {
        mapData[x][y] = water;
      }
    yOffset+= COLS;
    }  

  LoadQuadrant(44, 0);
  LoadQuadrant(44, 1);
  LoadQuadrant(2, 2);
  LoadQuadrant(44, 3);
}

void ScrollViewport(byte direction)
{  
  int x, y;
  int totalSize = viewportCharHeight * viewportCharWidth;

  CameraFollow();

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
      byte buffer[viewportCharWidth];
      int offset = 0;
      int offset2 = 2;
      
      int BufferAddress = (int) &buffer[0];
      int CharAddress = (int) &DoubleBufferChars[offset];
      int CharAddress2 = (int) &DoubleBufferChars[offset2];
      int ColorAddress = (int) &DoubleBufferColors[offset];
      int ColorAddress2 = (int) &DoubleBufferColors[offset2];

      for (y = 0; y < viewportCharHeight; ++y)
      {        
        CopyMemory(BufferAddress, CharAddress, length);
        CopyMemory(CharAddress2, BufferAddress, length);
        CopyMemory(BufferAddress, ColorAddress, length);
        CopyMemory(ColorAddress2, BufferAddress, length);
        
        CharAddress += viewportCharWidth;
        CharAddress2 += viewportCharWidth;
        ColorAddress += viewportCharWidth;
        ColorAddress2 += viewportCharWidth;
      }
      DrawSingleColumn(0);
    }
    break;
    case 3:
    {
      int length = viewportCharWidth - 2;
      int offset = 0;
      int offset2 = 2;
      
      int CharAddress = (int) &DoubleBufferChars[offset];
      int CharAddress2 = (int) &DoubleBufferChars[offset2];
      int ColorAddress = (int) &DoubleBufferColors[offset];
      int ColorAddress2 = (int) &DoubleBufferColors[offset2];

      for (y = 0; y < viewportCharHeight; ++y)
      {
        CopyMemory(CharAddress, CharAddress2, length);
        CopyMemory(ColorAddress, ColorAddress2, length);
        CharAddress += viewportCharWidth;
        CharAddress2 += viewportCharWidth;
        ColorAddress += viewportCharWidth;
        ColorAddress2 += viewportCharWidth;
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
  
  //Check the tile we're already standing on
  if(ReadBit(tiles[mapData[xPos][yPos]].blocked, Direction))
  {
    WriteLineMessageWindow("Standing on blocked@", 1);
    return true;
  }
  
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
      xPos = wrapX(xPos - 1);
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
  {
    char message[16];
    WriteLineMessageWindow("Entry blocked@", 1);
    sprintf(message, "Index: %d@", tiles[mapData[xPos][yPos]].index);
    WriteLineMessageWindow(message, 1);
    sprintf(message, "Data: %d@", tiles[mapData[xPos][yPos]].blocked);
    WriteLineMessageWindow(message, 1);
    sprintf(message, "position: %d,%d@", xPos, yPos);
    WriteLineMessageWindow(message, 1);
    return true;
  }
  
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
  int x, y, a, b;

  CameraFollow();

  a = offsetX;
  b = offsetY;

  //Buffer the matrix of tiles for our viewport
  for(y = 0; y < viewportHeight; ++y)
  {
    //Wrap the map data y reference
    b = WrapMapPositionY(b);
      
    for(x = 0; x < viewportWidth; ++x)
      {
        //Wrap the map data X reference
        a = WrapMapPositionX(a);
        viewportBuffer[x][y] = mapData[a][b];
        DrawBufferTile(viewportBuffer[x][y], x, y);
      	a++;
      }
    a = offsetX;
    ++b;
  }

  //Update the viewport
  {
    UpdateViewport();
  }
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
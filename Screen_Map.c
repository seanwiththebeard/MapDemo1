#include <stdio.h>
#include <peekpoke.h>
#include "System_Graphics.h"
#include "Common.h"
#include "System_Input.h"
#include "System_MessageWindow.h"
#include "System_CharacterSets.h"
#include "System_MusicPlayer.h"
#include "Screen_AddCharacter.h"

//Globals
char str[16];
int int_offset, tileAddress, colorAddress;
byte byte_x, byte_y, byte_z, byte_a, byte_b, byte_temp, byte_i, checkCollision;
int int_x, int_y, int_index, int_a, int_b, xPos, yPos, chardata;
bool scrollQuads, changedQuad;
byte byte_index, byte_offset;


//Map Data
#define mapHeight 32
#define mapWidth 32
byte mapData[mapWidth][mapHeight];
#define mapQuadWidth 8
#define mapQuadHeight 8
#define mapMatrixWidth 8
#define mapMatrixHeight 8
byte mapQuads[mapMatrixHeight][mapMatrixWidth] = { //Map Data
  { 0,  1,  2,  3,  4,  5,  6,  7},
  { 8,  9, 10, 11, 12, 13, 14, 15},
  {16, 17, 18, 19, 20, 21, 22, 23},
  {24, 25, 26, 27, 28, 29, 30, 31},
  {32, 33, 34, 35, 36, 37, 38, 39},
  {40, 41, 42, 43, 44, 45, 46, 47},
  {48, 49, 50, 51, 52, 53, 54, 55},
  {56, 57, 58, 59, 60, 61, 62, 63}
};
byte quadBuffer[4];
int quadX = 0;
int quadY = 0;
#define quadWidth 8
#define quadHeight 8
const byte quadWidthDouble = quadWidth * 2;
const byte quadHeightDouble = quadHeight * 2;
const byte yQuadHeight = 2*mapQuadHeight;


bool wrap = true;

//Viewport
#define viewportPosX 0
#define viewportPosY 0
#define viewportWidth 9
#define viewportHeight 9
#define viewportCharWidth (viewportWidth * 2)
#define viewportCharHeight (viewportHeight * 2)
const byte doubleCharWidth = viewportCharWidth;
const byte doubleCharHeight = viewportCharHeight;
const byte viewportWidthQuad = (viewportWidth*4);
const byte LastMapScanline = (8*viewportPosY + 16*viewportHeight);

//Scrolling left and right line buffer
const byte bufferLength = viewportCharWidth - 2;
byte buffer[viewportCharWidth];
const int BufferAddress = (int) &buffer[0];
const int totalSize = viewportCharHeight * viewportCharWidth;
byte viewportBuffer[viewportWidth][viewportHeight];
byte DoubleBufferChars[viewportCharWidth*viewportCharHeight];
byte DoubleBufferColors[viewportCharWidth*viewportCharHeight];
#undef viewportCharWidth
#undef viewportCharHeight

int CharAddress, CharAddress2, ColorAddress, ColorAddress2;

byte followIndex = 0;

int viewportOrigin = (int)&ScreenDoubleBuffer[0];
int colorOrigin = (int)&ScreenDoubleBuffer[1000];

//QuadScroll
byte originX, originY;
byte quadA; //Entering quad
byte quadB; //Diagonal quad
byte indexA, indexB;
byte compareQuad;
bool posX, posY;

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

void DrawTile(byte tileIndex, byte tileX, byte tileY) //Draws directly to the graphics double-buffer
{
  int_offset = tileX * 2 + YColumnIndex[tileY] * 2;
  tileAddress = viewportOrigin + int_offset;
  colorAddress = colorOrigin + int_offset;
          
  POKEW(tileAddress, PEEKW(&tiles[tileIndex].chars[0]));
  POKEW(tileAddress + COLS, PEEKW(&tiles[tileIndex].chars[2]));
  POKEW(colorAddress, PEEKW(&tiles[tileIndex].colors[0]));
  POKEW(colorAddress + COLS, PEEKW(&tiles[tileIndex].colors[2]));
}

void DrawBufferTile(byte tileIndex, byte tileX, byte tileY) //Draws into the map viewport double-buffer
{
  int_offset = tileX * 2 + tileY*(viewportWidthQuad);
  tileAddress = (int) &DoubleBufferChars[int_offset];
  colorAddress = (int) &DoubleBufferColors[int_offset]; 

  POKEW(tileAddress, PEEKW(&tiles[tileIndex].chars[0]));
  POKEW(tileAddress + doubleCharWidth, PEEKW(&tiles[tileIndex].chars[2]));
  POKEW(colorAddress, PEEKW(&tiles[tileIndex].colors[0]));
  POKEW(colorAddress + doubleCharWidth, PEEKW(&tiles[tileIndex].colors[2]));
}

#define charactersCount 16
struct Character
{
  byte tile;
  byte chars[4];
  byte colors[4];
  byte trigger;
  byte combat;
  int posX;
  int posY;
  int quadPosX;
  int quadPosY;
  bool visible;
  bool collide;
  byte message;
} characters[charactersCount];


void CameraFollow()
{  
  offsetX = characters[followIndex].posX;
  offsetY = characters[followIndex].posY;
  
  for(byte_x = 0; byte_x < cameraOffsetX; ++byte_x)
  {
    --offsetX;
    if(offsetX >= mapWidth)
      offsetX = 0;
    if (offsetX < 0)
      offsetX = mapWidth - 1;
  }
  
  for(byte_y = 0; byte_y < cameraOffsetY; ++byte_y)
  {
    --offsetY;
    if(offsetY >= mapHeight)
      offsetY = 0;
    if (offsetY < 0)
      offsetY = mapHeight - 1;
  }
}

int GetWrappedX(int xPos) //For viewport character positions
{ 
  byte_temp = xPos - offsetX;
  
  if (xPos < offsetX)
    byte_temp += mapWidth;

  return byte_temp;
}

int GetWrappedY(int YPos)
{
  byte_temp = YPos - offsetY;
  
  if (YPos < offsetY)
    byte_temp += mapHeight;

  return byte_temp;
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

void BufferCharacters()
{
  for(byte_i = 0; byte_i < charactersCount; ++byte_i)
  {
    byte_x = GetWrappedX(characters[byte_i].posX);
    if (byte_x < viewportWidth)
    {
      byte_y = GetWrappedY(characters[byte_i].posY);
      if (byte_y < viewportHeight)
        if(characters[byte_i].visible)
          DrawTile(characters[byte_i].tile, byte_x, byte_y);
    }
  }
}

void UpdateViewport() //Copies the viewport buffer to the screen buffer
{
  for (int_x = 0; int_x < doubleCharHeight; ++int_x)
  {
    int_index = int_x * doubleCharWidth;
    int_offset = YColumnIndex[int_x];
    
    CopyMemory((int)(viewportOrigin + int_offset), (int) &DoubleBufferChars[int_index], doubleCharWidth);
    CopyMemory((int)(colorOrigin + int_offset), (int) &DoubleBufferColors[int_index], doubleCharWidth);    
  }
  BufferCharacters();
  //CopyDoubleBuffer();
  CopyDoubleBufferArea(viewportPosX, viewportPosY, doubleCharWidth, doubleCharHeight);
  //UpdateColors();
}

void DrawSingleRow(byte row)
{
  CameraFollow();
  
  byte_a = offsetX;
  byte_b = offsetY;
  
  for(byte_y = 0; byte_y < viewportHeight; ++byte_y)
  {    
    byte_b = WrapMapPositionY(byte_b);
    if (byte_y == row)
    {
      for(byte_x = 0; byte_x < viewportWidth; ++byte_x)
      {
        byte_a=WrapMapPositionX(byte_a);
        viewportBuffer[byte_x][byte_y] = mapData[byte_a][byte_b];
        DrawBufferTile(viewportBuffer[byte_x][byte_y], byte_x, byte_y);
      	byte_a++;
      }
      break;
    }
    byte_a = offsetX;
    ++byte_b;
  }
  UpdateViewport();
}

void DrawSingleColumn(byte column)
{
  CameraFollow();
  
  byte_a = offsetX;
  byte_b = offsetY;
  
  for(byte_y = 0; byte_y < viewportHeight; ++byte_y)
  {
    byte_b = WrapMapPositionY(byte_b);
    {
      for(byte_x = 0; byte_x < viewportWidth; ++byte_x)
      {
        byte_a = WrapMapPositionX(byte_a);
        viewportBuffer[byte_x][byte_y] = mapData[byte_a][byte_b];
        
        if (byte_x == column)
          DrawBufferTile(viewportBuffer[byte_x][byte_y], byte_x, byte_y);
        ++byte_a;
      }
    }
    byte_a = offsetX;
    ++byte_b;
  }
  UpdateViewport();
}

void FillQuadBuffer()
{
  quadX = characters[followIndex].quadPosX;
  quadY = characters[followIndex].quadPosY;

  if (quadX + 1 == mapQuadWidth)
    byte_x = 0;
  else
    byte_x = quadX + 1;

  if (quadY + 1 == mapQuadHeight)
    byte_y = 0;
  else
    byte_y = quadY + 1;

  quadBuffer[0] = mapQuads[quadY][quadX];
  quadBuffer[1] = mapQuads[quadY][byte_x];
  quadBuffer[2] = mapQuads[byte_y][quadX];
  quadBuffer[3] = mapQuads[byte_y][byte_x];
}

void LoadQuadrant(byte quadIndex, byte quad)
{  
  //sprintf(str, "Tile%d to Quad%d@", index, quad);
  //WriteLineMessageWindow(str, 1);
  
  quadBuffer[quad] = quadIndex;

  for (byte_z = 0; byte_z < 4; ++byte_z)
  {
    switch (quad)
    {
    case 0:
      originX = 0;
      originY = 0;
      break;
    case 1:
      originX = quadWidthDouble;
      originY = 0;
      break;
    case 2:
      originX = 0;
      originY = quadHeightDouble;
      break;
    case 3:
      originX = quadWidthDouble;
      originY = quadHeightDouble;
      break;
    default:
      break;
    }

    switch (byte_z)
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

    chardata = CharacterRam + 8*ScreenQuad[quadIndex].CharIndex[byte_z];
    for (byte_y = 0; byte_y < quadHeight; ++byte_y)
    {
      for (byte_x = 0; byte_x < quadWidth; ++byte_x)
      {
        if (ReadBit(PEEK(chardata + byte_y), 7 - byte_x) > 0)
        {
          mapData[byte_x + originX][byte_y + originY] = ScreenQuad[quadIndex].Chars[1];
        }
        else
        {
          mapData[byte_x + originX][byte_y + originY] = ScreenQuad[quadIndex].Chars[0];
        }
      }
    }
  }
}

void LoadMapQuads()
{
  FillQuadBuffer();

  LoadQuadrant(quadBuffer[0], 0);
  LoadQuadrant(quadBuffer[1], 1);
  LoadQuadrant(quadBuffer[2], 2);
  LoadQuadrant(quadBuffer[3], 3);
}

byte GetPlayerQuad() //Returns the viewport quadrant of the player character
{
  if (characters[followIndex].posX < 2 * mapQuadWidth)
  {
    if (characters[followIndex].posY < yQuadHeight)
      return 0;
    else
      return 2;
  }
  else
  {
    if (characters[followIndex].posY < yQuadHeight)
      return 1;
    else
      return 3;
  }
}

byte GetQuadInRelation(bool up, bool down, bool left, bool right)
{
  int_x = characters[followIndex].quadPosX;
  int_y = characters[followIndex].quadPosY;
  if (up)
  {
    --int_y;
    if (int_y < 0)
      int_y = mapMatrixHeight - 1;
  }
  if (down)
  {
    int_y++;
    if (int_y == mapMatrixHeight)
      int_y = 0;
  }
  if (left)
  {
    int_x--;
    if (int_x < 0)
      int_x = mapMatrixWidth - 1;
  }
  if (right)
  {
    ++int_x;
    if (int_x == mapMatrixWidth)
      int_x = 0;
  }
  return (mapQuads[int_y][int_x]);  
}

void QuadScroll(byte direction)
{
  SetChar(0, 24, 'p'); //Indicate processing
  
  originX = characters[followIndex].quadPosX;
  originY = characters[followIndex].quadPosY;
  compareQuad = GetPlayerQuad();
  
  posX = characters[followIndex].posX % 16 < quadWidth;
  posY = characters[followIndex].posY % 16 < quadHeight;
  
  switch(direction)
  {
    case 0:
      indexA = GetQuadInRelation(true, false, false, false);
      if (posX)
        indexB = GetQuadInRelation(true, false, true, false);
      else
        indexB = GetQuadInRelation(true, false, false, true);
      /*switch (compareQuad)
      {
        case 0:
          quadA = 2;
          quadB = 3;
          break;
        case 1:
          quadA = 3;
          quadB = 2;
          break;
        case 2:
          quadA = 0;
          quadB = 1;
          break;
        case 3:
          quadA = 1;
          quadB = 0;
          break;
      }*/
      break;
    case 1:
      indexA = GetQuadInRelation(false, true, false, false);
      if (posX)
        indexB = GetQuadInRelation(false, true, true, false);
      else
        indexB = GetQuadInRelation(false, true, false, true);
      /*switch (compareQuad)
      {
        case 0:
          quadA = 2;
          quadB = 3;
          break;
        case 1:
          quadA = 3;
          quadB = 2;
          break;
        case 2:
          quadA = 0;
          quadB = 1;
          break;
        case 3:
          quadA = 1;
          quadB = 0;
          break;
      }*/
      break;
    case 2:
      indexA = GetQuadInRelation(false, false, true, false);
      if (posY)
        indexB = GetQuadInRelation(true, false, true, false);
      else
        indexB = GetQuadInRelation(false, true, true, false);
      /*switch (compareQuad)
      {
        case 0:
          quadA = 1;
          quadB = 3;
          break;
        case 1:
          quadA = 0;
          quadB = 2;
          break;
        case 2:
          quadA = 3;
          quadB = 1;
          break;
        case 3:
          quadA = 2;
          quadB = 0;
          break;
      }*/
      break;
    case 3:
      indexA = GetQuadInRelation(false, false, false, true);
      if (posY)
        indexB = GetQuadInRelation(true, false, false, true);
      else
        indexB = GetQuadInRelation(false, true, false, true);
      /*switch (compareQuad)
      {
        case 0:
          quadA = 1;
          quadB = 3;
          break;
        case 1:
          quadA = 0;
          quadB = 2;
          break;
        case 2:
          quadA = 3;
          quadB = 1;
          break;
        case 3:
          quadA = 2;
          quadB = 0;
          break;
      }*/
      break;
  }
  
  if (direction < 2)
    switch (compareQuad)
      {
        case 0:
          quadA = 2;
          quadB = 3;
          break;
        case 1:
          quadA = 3;
          quadB = 2;
          break;
        case 2:
          quadA = 0;
          quadB = 1;
          break;
        case 3:
          quadA = 1;
          quadB = 0;
          break;
      }
  else
    switch (compareQuad)
      {
        case 0:
          quadA = 1;
          quadB = 3;
          break;
        case 1:
          quadA = 0;
          quadB = 2;
          break;
        case 2:
          quadA = 3;
          quadB = 1;
          break;
        case 3:
          quadA = 2;
          quadB = 0;
          break;
      }
  
  if (quadBuffer[quadA] != indexA)
      {
        LoadQuadrant(indexA, quadA);
      }
   if (quadBuffer[quadB] != indexB)
       {
  	LoadQuadrant(indexB, quadB);
       }
  UpdateViewport();
  SetChar(0, 24, ' '); //Indicate processing
  
}

void InitializeMapData()
{
  #define grass 36
  #define water 34
  #define signpost 35
  
  viewportOrigin += (viewportPosX + COLS * viewportPosY);
  colorOrigin += (viewportPosX + COLS * viewportPosY);
  
  cameraOffsetX = viewportWidth / 2;
  cameraOffsetY = viewportHeight / 2;
  
  for (byte_y = 0; byte_y < 8; ++byte_y)
    for (byte_x = 0; byte_x < 8; ++byte_x)
    {
      byte_index = byte_x + byte_y*8;
      byte_offset = byte_x * 2 + 32*byte_y;

      tiles[byte_index].index = byte_index; // Init tileset data for 64 tiles
      tiles[byte_index].chars[0] = byte_offset;
      tiles[byte_index].chars[1] = byte_offset + 1;
      tiles[byte_index].chars[2] = byte_offset + 16;
      tiles[byte_index].chars[3] = byte_offset + 17;

      tiles[byte_index].colors[0] = AttributeSet[byte_offset];
      tiles[byte_index].colors[1] = AttributeSet[byte_offset + 1];
      tiles[byte_index].colors[2] = AttributeSet[byte_offset + 16];
      tiles[byte_index].colors[3] = AttributeSet[byte_offset + 17];
      
      tiles[byte_index].blocked = 0;

      ScreenQuad[byte_index].CharIndex[0] = byte_offset; // Init screen quad prefabs for 8x8
      ScreenQuad[byte_index].CharIndex[1] = byte_offset + 1;
      ScreenQuad[byte_index].CharIndex[2] = byte_offset + 16;
      ScreenQuad[byte_index].CharIndex[3] = byte_offset + 17;
      ScreenQuad[byte_index].Chars[0] = 32;
      ScreenQuad[byte_index].Chars[1] = byte_index;
      ScreenQuad[byte_index].MusicIndex = 0;
      ScreenQuad[byte_index].NPCIndex = 0;
      ScreenQuad[byte_index].ScatterIndex = 0;
    }

    ScreenQuad[2].Chars[0] = 36; // Set the wizard to grass on 0
    ScreenQuad[2].Chars[1] = 44; // Set the wizard to trees on 1
  
  //Init Characters
  for (byte_i = 0; byte_i < charactersCount; ++byte_i)
  {
    characters[byte_i].tile = byte_i;
    characters[byte_i].chars[0] = 8 + byte_i * 16;
    characters[byte_i].chars[1] = 9 + byte_i * 16;
    characters[byte_i].chars[2] = 10 + byte_i * 16;
    characters[byte_i].chars[3] = 11 + byte_i * 16;
    characters[byte_i].colors[0] = byte_i + 1;
    characters[byte_i].colors[1] = byte_i + 1;
    characters[byte_i].colors[2] = byte_i + 1;
    characters[byte_i].colors[3] = byte_i + 1;
    characters[byte_i].posX = byte_i;
    characters[byte_i].posY = byte_i;
    characters[byte_i].quadPosX = byte_i;
    characters[byte_i].quadPosX = byte_i;
    characters[byte_i].visible = false;
    characters[byte_i].collide = false;
  }
    characters[0].visible = true;
    characters[0].posX  = 8;
    characters[0].posY  = 8;
    characters[0].quadPosX  = 2;
    characters[0].quadPosY  = 0;
    characters[0].tile = 2;

    characters[1].visible = true;
    characters[1].collide = true;
    characters[1].message = 0;

    characters[2].tile = signpost;
    characters[2].visible = true;
    characters[2].collide = true;
    characters[2].message = 2;
    characters[2].posX = 8;
    characters[2].posY = 6;
    characters[2].quadPosX  = 2;
    characters[2].quadPosY  = 0;

  
  /*//Init map data
  for(byte_y = 0; byte_y < mapHeight; ++byte_y)
    {
      for(byte_x = 0; byte_x < mapWidth; ++byte_x)
      {
        mapData[byte_x][byte_y] = water;
      }
    }  */
  LoadMapQuads();
}

void ScrollViewport(byte direction)
{
  
  CharAddress = (int) &DoubleBufferChars[0];
  CharAddress2 = (int) &DoubleBufferChars[2];
  ColorAddress = (int) &DoubleBufferColors[0];
  ColorAddress2 = (int) &DoubleBufferColors[2];
  
  CameraFollow();

  switch (direction)
  {
    case 0:
      {
        for (int_x = totalSize - viewportWidth * 4; int_x > 0; int_x -= viewportWidth * 4)
        {
          CopyMemory((int) &DoubleBufferChars[int_x], (int) &DoubleBufferChars[int_x - viewportWidth * 4], viewportWidthQuad);
          CopyMemory((int) &DoubleBufferColors[int_x], (int) &DoubleBufferColors[int_x - viewportWidth * 4], viewportWidthQuad);
        }
        DrawSingleRow(0);
      }
      break;
    case 1:
    {
      CopyMemory((int) &DoubleBufferChars[0], (int) &DoubleBufferChars[viewportWidth * 4], totalSize - viewportWidthQuad);
      CopyMemory((int) &DoubleBufferColors[0], (int) &DoubleBufferColors[viewportWidth * 4], totalSize - viewportWidthQuad);
      DrawSingleRow(viewportHeight - 1);
    }
    break;
    case 2:
    {
      for (byte_y = 0; byte_y < doubleCharHeight; ++byte_y)
      {        
        CopyMemory(BufferAddress, CharAddress, bufferLength);
        CopyMemory(CharAddress2, BufferAddress, bufferLength);
        CopyMemory(BufferAddress, ColorAddress, bufferLength);
        CopyMemory(ColorAddress2, BufferAddress, bufferLength);
        
        CharAddress += doubleCharWidth;
        CharAddress2 += doubleCharWidth;
        ColorAddress += doubleCharWidth;
        ColorAddress2 += doubleCharWidth;
      }
      DrawSingleColumn(0);
    }
    break;
    case 3:
    { 
      for (byte_y = 0; byte_y < doubleCharHeight; ++byte_y)
      {
        CopyMemory(CharAddress, CharAddress2, bufferLength);
        CopyMemory(ColorAddress, ColorAddress2, bufferLength);
        CharAddress += doubleCharWidth;
        CharAddress2 += doubleCharWidth;
        ColorAddress += doubleCharWidth;
        ColorAddress2 += doubleCharWidth;
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
  xPos = characters[charIndex].posX;
  yPos = characters[charIndex].posY;
  
  //Check the tile we're already standing on
  if(ReadBit(tiles[mapData[xPos][yPos]].blocked, Direction))
  {
    //WriteLineMessageWindow("Standing on blocked@", 0);
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
    /*WriteLineMessageWindow("Entry blocked@", 1);
    sprintf(str, "Index: %d@", tiles[mapData[xPos][yPos]].index);
    WriteLineMessageWindow(str, 1);
    sprintf(str, "Data: %d@", tiles[mapData[xPos][yPos]].blocked);
    WriteLineMessageWindow(str, 1);
    sprintf(str, "position: %d,%d@", xPos, yPos);
    WriteLineMessageWindow(str, 1);*/
    return true;
  }
  
  for (byte_i = 0; byte_i < charactersCount; ++byte_i)
    if(characters[byte_i].collide)
      if (characters[byte_i].posX == xPos)
        if (characters[byte_i].posY == yPos)
          {
            WriteLineMessageWindow(Messages[characters[byte_i].message], 1);
            return true;
          }
  
  return false;
}

void DrawEntireMap()
{
  CameraFollow();
  int_a = offsetX;
  int_b = offsetY;

  //Buffer the matrix of tiles for our viewport
  for(byte_y = 0; byte_y < viewportHeight; ++byte_y)
  {
    //Wrap the map data y reference
    int_b = WrapMapPositionY(int_b);
      
    for(byte_x = 0; byte_x < viewportWidth; ++byte_x)
      {
        //Wrap the map data X reference
        int_a = WrapMapPositionX(int_a);
        viewportBuffer[byte_x][byte_y] = mapData[int_a][int_b];
        DrawBufferTile(viewportBuffer[byte_x][byte_y], byte_x, byte_y);
      	int_a++;
      }
    int_a = offsetX;
    ++int_b;
  }

  //Update the viewport
  {
    UpdateViewport();
  }
}

void MoveCharacter(byte index, byte direction, bool cameraUpdate)
{
  checkCollision = CheckCollision(index, direction);
  scrollQuads = false;
  changedQuad = false;

  if(!checkCollision)
    {
      switch (direction)
      {
        case 0:
          --characters[index].posY;
          if (characters[index].posY < 0)
            characters[index].posY = mapHeight - 1;
          if (characters[index].posY == 15 || characters[index].posY == 31)
          {
            changedQuad = true;
            --characters[index].quadPosY;
            if(characters[index].quadPosY < 0)
              characters[index].quadPosY = mapMatrixHeight - 1;
          }
          break;
        case 1:
           ++characters[index].posY;
          if (characters[index].posY >= mapHeight)
            characters[index].posY = 0;
          if (characters[index].posY == 0 || characters[index].posY == 16)
          {
            changedQuad = true;
            ++characters[index].quadPosY;
            if(characters[index].quadPosY == mapMatrixHeight)
              characters[index].quadPosY = 0; 
          }
          break;
        case 2:
           --characters[index].posX;
           if (characters[index].posX < 0)
            characters[index].posX = mapWidth - 1;
            if (characters[index].posX == 15 || characters[index].posX == 31)
          {
            changedQuad = true;
            --characters[index].quadPosX;
            if(characters[index].quadPosX < 0)
              characters[index].quadPosX = mapMatrixWidth - 1; 
          }
           break;
        case 3:
           ++characters[index].posX;
           if (characters[index].posX >= mapWidth)
            characters[index].posX = 0;
          if (characters[index].posX == 0 || characters[index].posX == 16)
          {
            changedQuad = true;
            ++characters[index].quadPosX;
            if(characters[index].quadPosX == mapMatrixWidth)
              characters[index].quadPosX = 0; 
          }
          break;
        default:
          break;
      }

      if (index == followIndex)
      {
        if ((direction == 0 && characters[index].posY % 16 == 6)
        || (direction == 1  && characters[index].posY % 16 == 10)
        || (direction == 2 && characters[index].posX % 16 == 6)
        || (direction == 3 && characters[index].posX % 16 == 10))
            scrollQuads = true;
          
        
        if(cameraUpdate)
          CameraFollow();
        ScrollViewport(direction);
        
        if (scrollQuads)
          QuadScroll(direction);
        
        /*if (quadBuffer[GetPlayerQuad()] != mapQuads[characters[index].quadPosY][characters[index].quadPosX])
        {
          sprintf(str, "QuadPos X%d,Y%d@", characters[index].quadPosX, characters[index].quadPosY);
          WriteLineMessageWindow(str, 0);
          sprintf(str, "CharPos X%d,Y%d@", characters[index].posX, characters[index].posY);
          WriteLineMessageWindow(str, 0);
          sprintf(str, "Entering Quad %d@", GetPlayerQuad());
          WriteLineMessageWindow(str, 0);
          sprintf(str, "Quad Index %d@", quadBuffer[GetPlayerQuad()]);
          WriteLineMessageWindow(str, 0);
          sprintf(str, "Should be %d@",mapQuads[characters[index].quadPosY][characters[index].quadPosX]);
          WriteLineMessageWindow(str, 0);
          LoadQuadrant(mapQuads[characters[index].quadPosY][characters[index].quadPosX], GetPlayerQuad());
          DrawEntireMap();
        }*/
      }
  }
}

void LoadMap()
{
  ScreenDisable();
  InitializeMapData();
  DrawEntireMap();
  BlankMessageWindow();
  ScreenEnable();
}

byte CheckInput()
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
      DrawAddCharacterScreen();
      //LoadMap();
      //PlaySong();
      //sprintf(str, "Pos = %d,%d@", characters[0].posX, characters[0].posY);
      //DrawEntireMap();
      //WriteLineMessageWindow(str, 0);
      return 1;
    }
  return 0;
}
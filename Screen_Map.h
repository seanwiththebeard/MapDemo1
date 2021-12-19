#include "Common.h"

void InitializeMapData();
void DrawTile(byte index, byte xpos, byte ypos);
int DrawMap(bool blit);
void DrawCharacters();
void CheckInput();
void DrawCharacter(byte index, byte xpos, byte ypos);
void MapUpdate();

//Viewport
extern byte viewportPosX;
extern byte viewportPosY;
extern byte viewportWidth;
extern byte viewportHeight;
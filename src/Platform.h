#include "Common.h"

//Graphics
extern byte *CharRam;
extern byte *ScreenChars;
//#define ScreenRam 0xC800
#define CharacterRom 0xD000
#define ColorRam 0xD800

void ScrollingMaskOn(void);
void ScrollingMaskOff(void);
void ScrollReset(void);
void MoveScreenUp(void);
void MoveScreenDown(void);
void SetChar(byte x, byte y, byte index);
void UpdateColors(void);
void ClearScreen(void);
void DrawCharacterSet(byte destX, byte destY);
void CopyDoubleBuffer(void);
void CopyDoubleBufferArea(byte posX, byte posY, byte sizeX, byte sizeY);
void ReverseBufferArea(byte posX, byte posY, byte sizeX, byte sizeY);
void SelectVICBanks(byte bank, byte screenpos, byte charpos);
void SetTileOrigin(byte x, byte y);
void DrawTileFast(byte index, byte x, byte y);
void PrintString(char text[16], byte posx, byte posy, bool fast, bool buffer);
void DrawLineH(char index, byte x, byte y, byte length);
void DrawLineV(char index, byte x, byte y, byte length);
void DrawBorder(byte xPos, byte yPos, byte width, byte height, bool buffer, bool fill);
extern int YColumnIndex[];
extern byte *ScreenCharBuffer;
extern byte *ScreenColorBuffer;

//Audio
//extern char SIDFILE[];
extern void PlaySID(void);
extern void StopSID(void);
#define SIDSIZE (1024)*2
#define SIDLOAD 0xC000


//File I/O
void DiskSave(char filename[], int source, int length);
void DiskLoad(char filename[], int dest);

//Input
void InitializeInput();
void UpdateInput();
bool InputChanged();
bool NoInput();

bool InputUp();
bool InputDown(); 
bool InputLeft();
bool InputRight();
bool InputFire();
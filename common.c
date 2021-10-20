#include <peekpoke.h>
#include "common.h"
#include <string.h>
#include <c64.h>

byte ReadBit(byte byteToRead, char bit)
{
    bit = 1 << bit;
    return(bit & byteToRead);
}
void WriteBit(byte *byteToSet, char bit, bool value)
{
  if (value)
  {
    bit = 1 << bit;
  }
  else
  {
    bit = 0 << bit;
  }
  *byteToSet = *byteToSet | bit;
}

bool CheckBit(byte source, byte position)
{
  return (source & (1<<position));
}

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
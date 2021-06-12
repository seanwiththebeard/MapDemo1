#include <peekpoke.h>
#include "common.h"

void raster_wait(unsigned char line) {
  while (VIC.rasterline < line) ;
}

void wait_vblank(byte frames) 
{
  byte x;
  for (x = 0; x < frames; x++)
    raster_wait(255);
}

void ScreenDisable()
{
  POKE(0xD011, PEEK(0xD011)&239);
}

void ScreenEnable()
{
  POKE(0xD011, PEEK(0xD011)|16); 
}

void SetBackground(byte index)
{
  POKE(0xD021, index);
}

void SetBorder(byte index)
{
  POKE(0xD020, index);
}

int ReadBit(byte byteToRead, char bit)
{
    bit = 1 << bit;
    return(bit & byteToRead);
}
void WriteBit(byte *byteToSet, char bit, bool value)
{
  if (value)
  {
    bit = 1 << bit;
    *byteToSet = *byteToSet | bit;
  }
  else
  {
    bit = 0 << bit;
    *byteToSet = *byteToSet | bit;
  }
}
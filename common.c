#include <peekpoke.h>
#include "common.h"

void raster_wait(unsigned char line) {
  while (VIC.rasterline < line) ;
}

void wait_vblank(void) {
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
  POKE(0xD020, index);
}

void FlashColor(byte index, byte length)
{
  int i = 0;
  int retValue = PEEK(0xD021);
  POKE(0xD021, index);
  for (i = 0; i < length; i++)
    wait_vblank();
  POKE(0xD021, retValue);   
    
  //POKE(0xD021, index);
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
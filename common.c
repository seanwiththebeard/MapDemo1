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

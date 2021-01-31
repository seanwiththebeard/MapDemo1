#include <stdio.h>
#include <conio.h>
#include <c64.h>
#include <cbm_petscii_charmap.h>
#include <peekpoke.h>

void setbitmapmode()
{
  VIC.ctrl1 = 0x3b;
  VIC.ctrl2 = 0x08;
}

void settextmode()
{
  VIC.ctrl1 = 0x1b;
  VIC.ctrl2 = 0x08;
}

void setcolortextmode()
{
  int i = 0;
  
  //Copy character set
  POKE(0x0034, 48);  // Reserve RAM
  POKE(0x0038, 48); // Reserve RAM
  POKE(0xDC0E, PEEK(0xDC0E)&254); // Pause Keyscan
  POKE(0x0001, (PEEK(0x0001)&251)); // Character ROM select
  
  //Copy the character set to the new RAM location
  for(i = 0; i < 1024; i++)
  {
    POKE(i + 0x3000, PEEK(i + 0xD000));
  }
  
  POKE(0x0001, (PEEK(0x0001)|4)); // Character ROM de-select, back to IO
  POKE(0xDC0E, PEEK(0xDC0E)|1); // Resume Keyscan
  
  //Redirect the character set address, select uppercase set
  POKE (0xD018, (PEEK(0xD018)&240) +12);
  
  for(i = 0; i < 8; i++)
  {
    POKE(i + 0x3000, 255);
  }
  
  //Select first character set
  //POKE(0xD018, 21);
  //Select second character set
  //POKE(0xD018, 23);
  
  //Background / 00
  POKE(0xD021, 0);
  //Foreground / 01
  POKE(0xD022, 5);
  //Foreground / 10
  POKE(0xD023, 1);
  
  //Foreground / 11 / 0-7 single, 8-15 multi
  for (i = 0; i < 1000; i++)
    POKE(0xD800 + i, 1);
  
  
}
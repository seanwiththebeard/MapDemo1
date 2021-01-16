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
  //VIC.ctrl1 = 0x1b;
  //VIC.ctrl2 = 0x18;
  
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
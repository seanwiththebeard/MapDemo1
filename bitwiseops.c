#include <common.h>

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
#include <common.h>

void WriteBit(byte *byteToSet, char bit)
{
    bit = 1 << bit;
    *byteToSet = *byteToSet | bit;
}

int ReadBit(byte byteToRead, char bit)
{
    bit = 1 << bit;
    return(bit & byteToRead);
}
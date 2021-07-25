#ifndef _COMMON_H
#define _COMMON_H

#include <c64.h>
#include <stdint.h>

typedef uint8_t byte;
typedef uint16_t word;
typedef int8_t sbyte;
typedef enum { false, true } bool;
typedef struct{
  unsigned int value : 4;
}halfbyte;

#define COLS 40
#define ROWS 25

void CopyMemory(int dest, int src, int length); /*Absolute address, &pointer to source)*/
void raster_wait(unsigned char line);
void wait_vblank(byte frames);
void ScreenDisable(void);
void ScreenEnable(void);
void SetBackground(byte index);
void SetBorder(byte index);

void WriteBit(byte *byteToSet, char bit, bool value);
int ReadBit(byte byteToRead, char bit);
#endif

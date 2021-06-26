#include <c64.h>
#include <peekpoke.h>
#include "common.h"
#include "System_CharacterSets.h"

int CharacterRAM = 0xC000;

byte CharacterSet[2][2048]={
{0x00,0x11,0x1F,0x08,0x11,0x1F,0x11,0x0C,0x00,0x88,0xF8,0x10,0x88,0xF8,0x88,0x30
,0x0F,0x0B,0x0B,0x1F,0x0E,0x12,0x0E,0x00,0xF0,0xD0,0xD0,0xF8,0x70,0x48,0x70,0x00
,0x00,0x10,0x38,0x45,0x39,0x2B,0x3B,0x28,0x00,0x08,0x1C,0xA2,0x9C,0xD4,0xDC,0x14
,0x10,0x38,0x44,0x38,0x2B,0x3A,0x3A,0x00,0x08,0x1C,0x22,0x1C,0xD4,0x5C,0x5C,0x00
,0x00,0x1F,0x2F,0x08,0x0B,0x09,0x0A,0x18,0x00,0xE4,0xF6,0x14,0xD6,0x92,0x52,0x1A
,0x3C,0x3E,0x3F,0x06,0x16,0x06,0x06,0x00,0x3A,0x70,0xE6,0x66,0x60,0x62,0x62,0x00
,0x00,0x07,0x0C,0x0A,0x0B,0x0D,0x1B,0x10,0x00,0xE0,0x30,0x10,0x10,0xB0,0xD8,0x08
,0x03,0x17,0x17,0x10,0x16,0x07,0x07,0x00,0xC0,0xE8,0xE8,0x08,0x68,0xE0,0xE0,0x00
,0x00,0x03,0x04,0x03,0x05,0x07,0x03,0x18,0x00,0xC0,0x20,0xC0,0xA0,0xE0,0xC0,0x18
,0x3E,0x36,0x06,0x30,0x37,0x04,0x04,0x00,0x7C,0x6C,0x60,0x0C,0xEC,0x20,0x20,0x00
,0x00,0x03,0x07,0x07,0x05,0x06,0x09,0x18,0x00,0xC0,0xE0,0xE0,0xA0,0x60,0x90,0x18
,0x3E,0x34,0x34,0x00,0x17,0x04,0x04,0x00,0x7C,0x2C,0x2C,0x00,0xE8,0x20,0x20,0x00
,0x00,0x07,0x0F,0x0C,0x08,0x09,0x0A,0x1B,0x00,0xE0,0xF0,0x30,0x10,0x90,0x50,0xD8
,0x3B,0x35,0x36,0x36,0x26,0x16,0x06,0x00,0xDC,0xAC,0x6C,0x6C,0x64,0x68,0x60,0x00
,0x00,0x03,0x07,0x07,0x05,0x17,0x29,0x24,0x00,0xC0,0xE0,0xE0,0xA0,0xE8,0x94,0x24
,0x37,0x30,0x27,0x27,0x34,0x30,0x04,0x00,0xEC,0x0C,0xE4,0xE4,0x2C,0x0C,0x20,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x00,0x00,0x18,0x00
,0x66,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x66,0x66,0xFF,0x66,0xFF,0x66,0x66,0x00
,0x18,0x3E,0x60,0x3C,0x06,0x7C,0x18,0x00,0x62,0x66,0x0C,0x18,0x30,0x66,0x46,0x00
,0x3C,0x66,0x3C,0x38,0x67,0x66,0x3F,0x00,0x06,0x0C,0x18,0x00,0x00,0x00,0x00,0x00
,0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00,0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00
,0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x03,0x06,0x0C,0x18,0x30,0x60,0x00
,0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00,0x18,0x18,0x38,0x18,0x18,0x18,0x7E,0x00
,0x3C,0x66,0x06,0x0C,0x30,0x60,0x7E,0x00,0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00
,0x06,0x0E,0x1E,0x66,0x7F,0x06,0x06,0x00,0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00
,0x3C,0x66,0x60,0x7C,0x66,0x66,0x3C,0x00,0x7E,0x66,0x0C,0x18,0x18,0x18,0x18,0x00
,0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00,0x3C,0x66,0x66,0x3E,0x06,0x66,0x3C,0x00
,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x18,0x18,0x30
,0x0E,0x18,0x30,0x60,0x30,0x18,0x0E,0x00,0x00,0x00,0x7E,0x00,0x7E,0x00,0x00,0x00
,0x70,0x18,0x0C,0x06,0x0C,0x18,0x70,0x00,0x3C,0x66,0x06,0x0C,0x18,0x00,0x18,0x00
,0x3C,0x66,0x6E,0x6E,0x60,0x62,0x3C,0x00,0x00,0x00,0x3C,0x06,0x3E,0x66,0x3E,0x00
,0x00,0x60,0x60,0x7C,0x66,0x66,0x7C,0x00,0x00,0x00,0x3C,0x60,0x60,0x60,0x3C,0x00
,0x00,0x06,0x06,0x3E,0x66,0x66,0x3E,0x00,0x00,0x00,0x3C,0x66,0x7E,0x60,0x3C,0x00
,0x00,0x0E,0x18,0x3E,0x18,0x18,0x18,0x00,0x00,0x00,0x3E,0x66,0x66,0x3E,0x06,0x7C
,0x00,0x60,0x60,0x7C,0x66,0x66,0x66,0x00,0x00,0x18,0x00,0x38,0x18,0x18,0x3C,0x00
,0x00,0x06,0x00,0x06,0x06,0x06,0x06,0x3C,0x00,0x60,0x60,0x6C,0x78,0x6C,0x66,0x00
,0x00,0x38,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x66,0x7F,0x7F,0x6B,0x63,0x00
,0x00,0x00,0x7C,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x3C,0x66,0x66,0x66,0x3C,0x00
,0x00,0x00,0x7C,0x66,0x66,0x7C,0x60,0x60,0x00,0x00,0x3E,0x66,0x66,0x3E,0x06,0x06
,0x00,0x00,0x7C,0x66,0x60,0x60,0x60,0x00,0x00,0x00,0x3E,0x60,0x3C,0x06,0x7C,0x00
,0x00,0x18,0x7E,0x18,0x18,0x18,0x0E,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x3E,0x00
,0x00,0x00,0x66,0x66,0x66,0x3C,0x18,0x00,0x00,0x00,0x63,0x6B,0x7F,0x3E,0x36,0x00
,0x00,0x00,0x66,0x3C,0x18,0x3C,0x66,0x00,0x00,0x00,0x66,0x66,0x66,0x3E,0x0C,0x78
,0x00,0x00,0x7E,0x0C,0x18,0x30,0x7E,0x00,0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00
,0x0C,0x12,0x30,0x7C,0x30,0x62,0xFC,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00
,0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x18,0x00,0x10,0x30,0x7F,0x7F,0x30,0x10,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
,0x00,0x00,0x08,0x2A,0x00,0x00,0x00,0x00,0x00,0x20,0x32,0x16,0x04,0x00,0x00,0x00
,0x04,0x4C,0x68,0x20,0x00,0x01,0x05,0x00,0x08,0x2A,0x00,0x00,0x00,0x00,0x40,0x00
,0x00,0x6F,0x6F,0x60,0x6D,0x6D,0x01,0x6D,0x00,0xB6,0xB6,0x30,0xB6,0xB6,0x80,0xBE
,0x6D,0x0C,0x6D,0x6D,0x60,0x6F,0x6F,0x00,0xBE,0x00,0xF6,0xF6,0x06,0xB6,0xB6,0x00
,0x00,0x3B,0x5B,0x6A,0x71,0x00,0x71,0x6A,0x00,0x8E,0x56,0xDA,0xDC,0x00,0xDC,0xDA
,0x5B,0x3B,0x00,0x3B,0x5B,0x6A,0x71,0x00,0x56,0x8E,0x00,0x8E,0x56,0xDA,0xDC,0x00
,0x00,0x01,0x00,0x1F,0x1F,0x18,0x1F,0x18,0x00,0x80,0x00,0xF8,0xF8,0x18,0xF8,0x18
,0x1F,0x1F,0x00,0x01,0x01,0x09,0x05,0x00,0xF8,0xF8,0x00,0x80,0x80,0x90,0xA0,0x00
,0x00,0x7F,0x7F,0x7F,0x40,0x5A,0x5A,0x5A,0x00,0xFE,0xFE,0xFE,0x02,0x2A,0x4A,0x8A
,0x7F,0x40,0x54,0x52,0x51,0x7F,0x00,0x00,0xFE,0x02,0x5A,0x5A,0x5A,0xFE,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x7E,0x7E,0x7E,0x00,0x00,0x78,0xF8,0x58,0xF2,0xC6,0x1E
,0x7F,0x40,0x54,0x52,0x51,0x7F,0x00,0x00,0xFE,0x02,0x5A,0x5A,0x5A,0xFE,0x00,0x00
,0x00,0x3F,0x60,0x5F,0x50,0x57,0x57,0x57,0x00,0xFC,0x06,0xFA,0x0A,0xEA,0xEA,0xEA
,0x50,0x5F,0x50,0x57,0x57,0x50,0x5F,0x00,0x0A,0xDA,0x0A,0xEA,0xEA,0x0A,0xFA,0x00
,0x00,0x00,0x00,0x01,0x03,0x03,0x37,0x37,0x00,0x00,0x00,0xF0,0xF8,0xF8,0xFC,0xFC
,0x03,0x00,0x4F,0x1F,0x1F,0x0F,0x00,0x00,0xF8,0x00,0x9A,0xD8,0xC0,0x80,0x00,0x00
,0x00,0x00,0x00,0x00,0x08,0x08,0x1C,0x1D,0x00,0x20,0x20,0x70,0x70,0xF8,0xF8,0xFC
,0x3E,0x3E,0x7F,0x7F,0x3E,0x00,0x08,0x00,0xFC,0xFE,0x7E,0x7C,0x00,0x20,0x00,0x00
,0xFF,0x80,0xBF,0xA0,0xAA,0xA5,0xAA,0xA5,0xFF,0x01,0xFD,0x05,0xA5,0x55,0xA5,0x55
,0xAA,0xA5,0xAA,0xA5,0xA0,0xBF,0x80,0xFF,0xA5,0x55,0xA5,0x55,0x05,0xFD,0x01,0xFF
,0xFF,0x80,0xBF,0xBF,0xB0,0xB0,0xB0,0xB0,0xFF,0x02,0xFA,0xF2,0x12,0x12,0x12,0x12
,0xB0,0xB0,0xB0,0xBF,0xA0,0x80,0xFF,0x80,0x12,0x12,0x12,0xF2,0x02,0x02,0xFE,0x00
,0x00,0x70,0x70,0x76,0x76,0x76,0x76,0x46,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xD8
,0x46,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0xD8,0xDA,0xDA,0x1A,0x1A,0x02,0xFE,0x00
,0x00,0x00,0x3F,0x20,0x2C,0x2C,0x2D,0x2D,0x00,0x00,0xFC,0x04,0x04,0x04,0x84,0x84
,0x2D,0x2D,0x2D,0x2D,0x20,0x3F,0x00,0x00,0xB4,0xB4,0xB4,0xB4,0x04,0xFC,0x00,0x00
,0x00,0x78,0x4A,0x7B,0x07,0x37,0x30,0x00,0x00,0x1E,0x52,0xDE,0xE0,0xEC,0x0C,0x00
,0x78,0x48,0x7A,0x07,0x37,0x36,0x36,0x00,0x1E,0x12,0x5E,0xE0,0xEC,0x6C,0x6C,0x00
,0x3C,0x66,0x6E,0x6E,0x60,0x62,0x3C,0x00,0x18,0x3C,0x66,0x7E,0x66,0x66,0x66,0x00
,0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00,0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00
,0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00,0x7E,0x60,0x60,0x78,0x60,0x60,0x7E,0x00
,0x7E,0x60,0x60,0x78,0x60,0x60,0x60,0x00,0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00
,0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00
,0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00,0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00
,0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00,0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00
,0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0x00,0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00
,0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00,0x3C,0x66,0x66,0x66,0x66,0x3C,0x0E,0x00
,0x7C,0x66,0x66,0x7C,0x78,0x6C,0x66,0x00,0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00
,0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00
,0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00,0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00
,0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00,0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00
,0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
},
  {0x3C,0x66,0x6E,0x6E,0x60,0x62,0x3C,0x00,0x18,0x3C,0x66,0x7E,0x66,0x66,0x66,0x00
, 0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00,0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00
, 0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00,0x7E,0x60,0x60,0x78,0x60,0x60,0x7E,0x00
, 0x7E,0x60,0x60,0x78,0x60,0x60,0x60,0x00,0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00
, 0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00
, 0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00,0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00
, 0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00,0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00
, 0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0x00,0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00
, 0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00,0x3C,0x66,0x66,0x66,0x66,0x3C,0x0E,0x00
, 0x7C,0x66,0x66,0x7C,0x78,0x6C,0x66,0x00,0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00
, 0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00
, 0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00,0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00
, 0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00,0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00
, 0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00,0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00
, 0x0C,0x12,0x30,0x7C,0x30,0x62,0xFC,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00
, 0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x18,0x00,0x10,0x30,0x7F,0x7F,0x30,0x10,0x00
, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x00,0x00,0x18,0x00
, 0x66,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x66,0x66,0xFF,0x66,0xFF,0x66,0x66,0x00
, 0x18,0x3E,0x60,0x3C,0x06,0x7C,0x18,0x00,0x62,0x66,0x0C,0x18,0x30,0x66,0x46,0x00
, 0x3C,0x66,0x3C,0x38,0x67,0x66,0x3F,0x00,0x06,0x0C,0x18,0x00,0x00,0x00,0x00,0x00
, 0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00,0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00
, 0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00
, 0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00
, 0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x03,0x06,0x0C,0x18,0x30,0x60,0x00
, 0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00,0x18,0x18,0x38,0x18,0x18,0x18,0x7E,0x00
, 0x3C,0x66,0x06,0x0C,0x30,0x60,0x7E,0x00,0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00
, 0x06,0x0E,0x1E,0x66,0x7F,0x06,0x06,0x00,0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00
, 0x3C,0x66,0x60,0x7C,0x66,0x66,0x3C,0x00,0x7E,0x66,0x0C,0x18,0x18,0x18,0x18,0x00
, 0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00,0x3C,0x66,0x66,0x3E,0x06,0x66,0x3C,0x00
, 0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x18,0x18,0x30
, 0x0E,0x18,0x30,0x60,0x30,0x18,0x0E,0x00,0x00,0x00,0x7E,0x00,0x7E,0x00,0x00,0x00
, 0x70,0x18,0x0C,0x06,0x0C,0x18,0x70,0x00,0x3C,0x66,0x06,0x0C,0x18,0x00,0x18,0x00
, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
, 0x00,0x00,0x08,0x2A,0x00,0x00,0x00,0x00,0x00,0x20,0x32,0x16,0x04,0x00,0x00,0x00
, 0x04,0x4C,0x68,0x20,0x00,0x01,0x05,0x00,0x08,0x2A,0x00,0x00,0x00,0x00,0x40,0x00
, 0x00,0x6F,0x6F,0x60,0x6D,0x6D,0x01,0x6D,0x00,0xB6,0xB6,0x30,0xB6,0xB6,0x80,0xBE
, 0x6D,0x0C,0x6D,0x6D,0x60,0x6F,0x6F,0x00,0xBE,0x00,0xF6,0xF6,0x06,0xB6,0xB6,0x00
, 0x00,0x3B,0x5B,0x6A,0x71,0x00,0x71,0x6A,0x00,0x8E,0x56,0xDA,0xDC,0x00,0xDC,0xDA
, 0x5B,0x3B,0x00,0x3B,0x5B,0x6A,0x71,0x00,0x56,0x8E,0x00,0x8E,0x56,0xDA,0xDC,0x00
, 0x00,0x01,0x00,0x1F,0x1F,0x18,0x1F,0x18,0x00,0x80,0x00,0xF8,0xF8,0x18,0xF8,0x18
, 0x1F,0x1F,0x00,0x01,0x01,0x09,0x05,0x00,0xF8,0xF8,0x00,0x80,0x80,0x90,0xA0,0x00
, 0x00,0x7F,0x7F,0x7F,0x40,0x5A,0x5A,0x5A,0x00,0xFE,0xFE,0xFE,0x02,0x2A,0x4A,0x8A
, 0x7F,0x40,0x54,0x52,0x51,0x7F,0x00,0x00,0xFE,0x02,0x5A,0x5A,0x5A,0xFE,0x00,0x00
, 0x00,0x00,0x00,0x00,0x00,0x7E,0x7E,0x7E,0x00,0x00,0x78,0xF8,0x58,0xF2,0xC6,0x1E
, 0x7F,0x40,0x54,0x52,0x51,0x7F,0x00,0x00,0xFE,0x02,0x5A,0x5A,0x5A,0xFE,0x00,0x00
, 0x00,0x3F,0x60,0x5F,0x50,0x57,0x57,0x57,0x00,0xFC,0x06,0xFA,0x0A,0xEA,0xEA,0xEA
, 0x50,0x5F,0x50,0x57,0x57,0x50,0x5F,0x00,0x0A,0xDA,0x0A,0xEA,0xEA,0x0A,0xFA,0x00
, 0x00,0x00,0x00,0x01,0x03,0x03,0x37,0x37,0x00,0x00,0x00,0xF0,0xF8,0xF8,0xFC,0xFC
, 0x03,0x00,0x4F,0x1F,0x1F,0x0F,0x00,0x00,0xF8,0x00,0x9A,0xD8,0xC0,0x80,0x00,0x00
, 0x00,0x00,0x00,0x00,0x08,0x08,0x1C,0x1D,0x00,0x20,0x20,0x70,0x70,0xF8,0xF8,0xFC
, 0x3E,0x3E,0x7F,0x7F,0x3E,0x00,0x08,0x00,0xFC,0xFE,0x7E,0x7C,0x00,0x20,0x00,0x00
, 0xFF,0x80,0xBF,0xA0,0xAA,0xA5,0xAA,0xA5,0xFF,0x01,0xFD,0x05,0xA5,0x55,0xA5,0x55
, 0xAA,0xA5,0xAA,0xA5,0xA0,0xBF,0x80,0xFF,0xA5,0x55,0xA5,0x55,0x05,0xFD,0x01,0xFF
, 0xFF,0x80,0xBF,0xBF,0xB0,0xB0,0xB0,0xB0,0xFF,0x02,0xFA,0xF2,0x12,0x12,0x12,0x12
, 0xB0,0xB0,0xB0,0xBF,0xA0,0x80,0xFF,0x80,0x12,0x12,0x12,0xF2,0x02,0x02,0xFE,0x00
, 0x00,0x70,0x70,0x76,0x76,0x76,0x76,0x46,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xD8
, 0x46,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0xD8,0xDA,0xDA,0x1A,0x1A,0x02,0xFE,0x00
, 0x00,0x00,0x3F,0x20,0x2C,0x2C,0x2D,0x2D,0x00,0x00,0xFC,0x04,0x04,0x04,0x84,0x84
, 0x2D,0x2D,0x2D,0x2D,0x20,0x3F,0x00,0x00,0xB4,0xB4,0xB4,0xB4,0x04,0xFC,0x00,0x00
, 0x00,0x78,0x4A,0x7B,0x07,0x37,0x30,0x00,0x00,0x1E,0x52,0xDE,0xE0,0xEC,0x0C,0x00
, 0x78,0x48,0x7A,0x07,0x37,0x36,0x36,0x00,0x1E,0x12,0x5E,0xE0,0xEC,0x6C,0x6C,0x00
, 0x00,0x11,0x1F,0x08,0x11,0x1F,0x11,0x0C,0x00,0x88,0xF8,0x10,0x88,0xF8,0x88,0x30
, 0x0F,0x0B,0x0B,0x1F,0x0E,0x12,0x0E,0x00,0xF0,0xD0,0xD0,0xF8,0x70,0x48,0x70,0x00
, 0x00,0x10,0x38,0x45,0x39,0x2B,0x3B,0x28,0x00,0x08,0x1C,0xA2,0x9C,0xD4,0xDC,0x14
, 0x10,0x38,0x44,0x38,0x2B,0x3A,0x3A,0x00,0x08,0x1C,0x22,0x1C,0xD4,0x5C,0x5C,0x00
, 0x00,0x1F,0x2F,0x08,0x0B,0x09,0x0A,0x18,0x00,0xE4,0xF6,0x14,0xD6,0x92,0x52,0x1A
, 0x3C,0x3E,0x3F,0x06,0x16,0x06,0x06,0x00,0x3A,0x70,0xE6,0x66,0x60,0x62,0x62,0x00
, 0x00,0x07,0x0C,0x0A,0x0B,0x0D,0x1B,0x10,0x00,0xE0,0x30,0x10,0x10,0xB0,0xD8,0x08
, 0x03,0x17,0x17,0x10,0x16,0x07,0x07,0x00,0xC0,0xE8,0xE8,0x08,0x68,0xE0,0xE0,0x00
, 0x00,0x03,0x04,0x03,0x05,0x07,0x03,0x18,0x00,0xC0,0x20,0xC0,0xA0,0xE0,0xC0,0x18
, 0x3E,0x36,0x06,0x30,0x37,0x04,0x04,0x00,0x7C,0x6C,0x60,0x0C,0xEC,0x20,0x20,0x00
, 0x00,0x03,0x07,0x07,0x05,0x06,0x09,0x18,0x00,0xC0,0xE0,0xE0,0xA0,0x60,0x90,0x18
, 0x3E,0x34,0x34,0x00,0x17,0x04,0x04,0x00,0x7C,0x2C,0x2C,0x00,0xE8,0x20,0x20,0x00
, 0x00,0x07,0x0F,0x0C,0x08,0x09,0x0A,0x1B,0x00,0xE0,0xF0,0x30,0x10,0x90,0x50,0xD8
, 0x3B,0x35,0x36,0x36,0x26,0x16,0x06,0x00,0xDC,0xAC,0x6C,0x6C,0x64,0x68,0x60,0x00
, 0x00,0x03,0x07,0x07,0x05,0x17,0x29,0x24,0x00,0xC0,0xE0,0xE0,0xA0,0xE8,0x94,0x24
, 0x37,0x30,0x27,0x27,0x34,0x30,0x04,0x00,0xEC,0x0C,0xE4,0xE4,0x2C,0x0C,0x20,0x00
, 0x00,0x05,0x07,0x00,0x07,0x05,0x02,0x18,0x00,0xA0,0xE0,0x00,0xE0,0xA0,0x40,0x18
, 0x3C,0x36,0x37,0x00,0x17,0x07,0x05,0x00,0x3C,0x6C,0xEC,0x00,0xE8,0xE0,0xA0,0x00
, 0x00,0x07,0x17,0x14,0x17,0x12,0x3A,0x78,0x00,0xE0,0xE8,0x28,0xE8,0x48,0x5C,0x1E
, 0x67,0x67,0x60,0x77,0x34,0x04,0x04,0x00,0xE6,0xE6,0x06,0xEE,0x2C,0x20,0x20,0x00
  }
};

byte AttributeSet[2][256]={
{0x0F,0x0F,0x0C,0x0C,0x08,0x08,0x0A,0x0A,0x0F,0x0F,0x0F,0x0F,0x01,0x01,0x01,0x01
,0x01,0x01,0x01,0x01,0x0A,0x0A,0x0A,0x0A,0x01,0x01,0x01,0x01,0x0F,0x0F,0x0C,0x0C
,0x03,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E
,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E
,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03
,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03
,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03
,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03
,0x03,0x03,0x03,0x03,0x0E,0x0E,0x0E,0x0E,0x0D,0x05,0x05,0x0D,0x02,0x08,0x08,0x02
,0x04,0x06,0x06,0x04,0x08,0x08,0x09,0x09,0x08,0x08,0x09,0x09,0x08,0x0F,0x09,0x09
,0x08,0x08,0x08,0x08,0x0C,0x0C,0x0B,0x0B,0x0D,0x0D,0x05,0x05,0x0B,0x0B,0x0B,0x0B
,0x0F,0x0C,0x0A,0x0C,0x0C,0x0C,0x0B,0x0C,0x0F,0x0B,0x0C,0x0B,0x0F,0x0F,0x0C,0x0C
,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E
,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E

},

{0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E
, 0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E
, 0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E
, 0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E
, 0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03
, 0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03
, 0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03
, 0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03
, 0x03,0x03,0x03,0x03,0x0E,0x0E,0x0E,0x0E,0x0D,0x05,0x05,0x0D,0x02,0x08,0x08,0x02
, 0x04,0x06,0x06,0x04,0x08,0x08,0x09,0x09,0x08,0x08,0x09,0x09,0x08,0x0F,0x09,0x09
, 0x08,0x08,0x08,0x08,0x0C,0x0C,0x0B,0x0B,0x0D,0x0D,0x05,0x05,0x0B,0x0B,0x0B,0x0B
, 0x0F,0x0C,0x0A,0x0C,0x0C,0x0C,0x0B,0x0C,0x0F,0x0B,0x0C,0x0B,0x0F,0x0F,0x0C,0x0C
, 0x0F,0x0F,0x0C,0x0C,0x08,0x08,0x0A,0x0A,0x0F,0x0F,0x0F,0x0F,0x01,0x01,0x01,0x01
, 0x01,0x01,0x01,0x01,0x0A,0x0A,0x0A,0x0A,0x01,0x01,0x01,0x01,0x0F,0x0F,0x0C,0x0C
, 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03
, 0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03}
};

void SetCharacterSet(byte index)
{
  int i;
  for (i = 0;i < 2048; i++)
  {
      POKE(CharacterRAM + i, CharacterSet[index][i]);  
  }
}

void UpdateCharacterSet(byte index)
{
  int i;
  for (i = 384;i < 2048; i++)
  {
      POKE(CharacterRAM + i + 384, CharacterSet[index][i]);  
  }
}
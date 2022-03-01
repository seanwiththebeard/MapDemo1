/*
From the 8bitworkshop samples
A simple music player.
*/

#include <string.h>
#include <c64.h>
#include <_sid.h>

#include "Common.h"

// SID frequency table (PAL version)
const int note_table_pal[96] = 
{
  278,295,313,331,351,372,394,417,442,468,496,526,557,590,625,
  662,702,743,788,834,884,937,992,1051,1114,1180,1250,1325,1403
    ,1487,1575,1669,1768,1873,1985,2103,2228,2360,2500,2649,2807
    ,2973,3150,3338,3536,3746,3969,4205,4455,4720,5001,5298,5613
    ,5947,6300,6675,7072,7493,7938,8410,8910,9440,10001,10596,11226
    ,11894,12601,13350,14144,14985,15876,16820,17820,18880,20003,21192
    ,22452,23787,25202,26700,28288,29970,31752,33640L,35641L,37760L
    ,40005L,42384L,44904L,47574L,50403L,53401L,56576L,59940L,63504L,67280L
};

#define NOTE_TABLE note_table_pal

// SID utility routines

void sid_init() {
  SID.flt_freq = 0xff;
  SID.flt_ctrl = 0xff;
  SID.amp = 0xff;
}

#define SID_PULSE_DECAY(voice, period) \
  SID.voice.ctrl = 0x00; \
  SID.voice.freq = period; \
  SID.voice.pw = 0x7f; \
  SID.voice.ad = 0x88; \
  SID.voice.sr = 0x48; \
  SID.voice.ctrl = 0x31; \

// music player logic

byte music_index = 0;
byte cur_duration = 0;

const byte music1[]; // music data -- see end of file
const byte* music_ptr = music1;

byte next_music_byte() {
  return *music_ptr++;
}

void play_music() {
  static byte chs = 0;
  if (music_ptr) {
    // run out duration timer yet?
    while (cur_duration == 0) {
      // fetch next byte in score
      byte note = next_music_byte();
      // is this a note?
      if ((note & 0x80) == 0) {
        int period = NOTE_TABLE[note & 63];
        // see which pulse generator is free
        if (!(chs & 1)) {
          SID_PULSE_DECAY(v1, period);
          chs |= 1;
        } else if (!(chs & 2)) {
          SID_PULSE_DECAY(v2, period);
          chs |= 2;
        } else if (!(chs & 4)) {
          SID_PULSE_DECAY(v3, period);
          chs |= 4;
        }
      } else {
        // end of score marker
        if (note == 0xff)
          music_ptr = NULL;
        // set duration until next note
        cur_duration = note & 63;
        // reset channel used mask
        chs = 0;
      }
    }
    cur_duration--;
  }
}

void start_music(const byte* music) {
  music_ptr = music;
  cur_duration = 0;
}

void music_update()
{
  //if(raster_check(0))
    if (music_ptr)
    {
      raster_wait(255);
      play_music();
    }
}

void PlaySong()
{
  sid_init();
  music_ptr = 0;
  //while (1)
  {
    if (!music_ptr)
      start_music(music1);
    wait_vblank(0);
    play_music();
  }
}

//
// MUSIC DATA -- "The Easy Winners" by Scott Joplin
//
const byte music1[] =
{
  0x00,0x07,0x0C,0x0A,0x0B,0x0D,0x1B,0x10,0x00,0xE0,0x30,0x10,0x10,0xB0,0xD8,0x08
,0x00,0x03,0x07,0x07,0x05,0x06,0x09,0x18,0x00,0xC0,0xE0,0xE0,0xA0,0x60,0x90,0x18
,0x00,0x1F,0x2F,0x08,0x0B,0x09,0x0A,0x18,0x00,0xE4,0xF6,0x14,0xD6,0x92,0x52,0x1A
,0x03,0x17,0x17,0x10,0x16,0x07,0x07,0x00,0xC0,0xE8,0xE8,0x08,0x68,0xE0,0xE0,0x00
,0x00,0x07,0x0F,0x0C,0x08,0x09,0x0A,0x1B,0x00,0xE0,0xF0,0x30,0x10,0x90,0x50,0xD8
,0x00,0x03,0x07,0x07,0x05,0x17,0x29,0x24,0x00,0xC0,0xE0,0xE0,0xA0,0xE8,0x94,0x24
,0x00,0x03,0x04,0x03,0x05,0x07,0x03,0x18,0x00,0xC0,0x20,0xC0,0xA0,0xE0,0xC0,0x18
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x3E,0x36,0x06,0x30,0x37,0x04,0x04,0x00,0x7C,0x6C,0x60,0x0C,0xEC,0x20,0x20,0x00
,0x3E,0x34,0x34,0x00,0x17,0x04,0x04,0x00,0x7C,0x2C,0x2C,0x00,0xE8,0x20,0x20,0x00
,0x3C,0x3E,0x3F,0x06,0x16,0x06,0x06,0x00,0x3A,0x70,0xE6,0x66,0x60,0x62,0x62,0x00
,0x3E,0x36,0x06,0x30,0x37,0x04,0x04,0x00,0x7C,0x6C,0x60,0x0C,0xEC,0x20,0x20,0x00
,0x3B,0x35,0x36,0x36,0x26,0x16,0x06,0x00,0xDC,0xAC,0x6C,0x6C,0x64,0x68,0x60,0x00
,0x37,0x30,0x27,0x27,0x34,0x30,0x04,0x00,0xEC,0x0C,0xE4,0xE4,0x2C,0x0C,0x20,0x00
,0x3E,0x36,0x06,0x30,0x37,0x04,0x04,0x00,0x7C,0x6C,0x60,0x0C,0xEC,0x20,0x20,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
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
,0x00,0x11,0x1F,0x08,0x11,0x1F,0x11,0x0C,0x00,0x88,0xF8,0x10,0x88,0xF8,0x88,0x30
,0x00,0x10,0x38,0x45,0x39,0x2B,0x3B,0x28,0x00,0x08,0x1C,0xA2,0x9C,0xD4,0xDC,0x14
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x0F,0x0B,0x0B,0x1F,0x0E,0x12,0x0E,0x00,0xF0,0xD0,0xD0,0xF8,0x70,0x48,0x70,0x00
,0x10,0x38,0x44,0x38,0x2B,0x3A,0x3A,0x00,0x08,0x1C,0x22,0x1C,0xD4,0x5C,0x5C,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x3B,0x5B,0x6A,0x71,0x00,0x71,0x6A,0x00,0x8E,0x56,0xDA,0xDC,0x00,0xDC,0xDA
,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
,0x00,0x01,0x00,0x1F,0x1F,0x18,0x1F,0x18,0x00,0x80,0x00,0xF8,0xF8,0x18,0xF8,0x18
,0x00,0x00,0x08,0x2A,0x00,0x00,0x00,0x00,0x00,0x20,0x32,0x16,0x04,0x00,0x00,0x00
,0x00,0x7F,0x7F,0x7F,0x40,0x5A,0x5A,0x5A,0x00,0xFE,0xFE,0xFE,0x02,0x2A,0x4A,0x8A
,0x00,0x6F,0x6F,0x60,0x6D,0x6D,0x01,0x6D,0x00,0xB6,0xB6,0x30,0xB6,0xB6,0x80,0xBE
,0x00,0x00,0x00,0x00,0x00,0x7E,0x7E,0x7E,0x00,0x00,0x78,0xF8,0x58,0xF2,0xC6,0x1E
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x5B,0x3B,0x00,0x3B,0x5B,0x6A,0x71,0x00,0x56,0x8E,0x00,0x8E,0x56,0xDA,0xDC,0x00
,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
,0x1F,0x1F,0x00,0x01,0x01,0x09,0x05,0x00,0xF8,0xF8,0x00,0x80,0x80,0x90,0xA0,0x00
,0x04,0x4C,0x68,0x20,0x00,0x01,0x05,0x00,0x08,0x2A,0x00,0x00,0x00,0x00,0x40,0x00
,0x7F,0x40,0x54,0x52,0x51,0x7F,0x00,0x00,0xFE,0x02,0x5A,0x5A,0x5A,0xFE,0x00,0x00
,0x6D,0x0C,0x6D,0x6D,0x60,0x6F,0x6F,0x00,0xBE,0x00,0xF6,0xF6,0x06,0xB6,0xB6,0x00
,0x7F,0x40,0x54,0x52,0x51,0x7F,0x00,0x00,0xFE,0x02,0x5A,0x5A,0x5A,0xFE,0x00,0x00
,0x00,0x3F,0x60,0x5F,0x50,0x57,0x57,0x57,0x00,0xFC,0x06,0xFA,0x0A,0xEA,0xEA,0xEA
,0xFF,0x80,0xBF,0xBF,0xB0,0xB0,0xB0,0xB0,0xFF,0x02,0xFA,0xF2,0x12,0x12,0x12,0x12
,0x00,0x00,0x00,0x01,0x03,0x03,0x37,0x37,0x00,0x00,0x00,0xF0,0xF8,0xF8,0xFC,0xFC
,0x00,0x70,0x70,0x76,0x76,0x76,0x76,0x46,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xD8
,0x00,0x00,0x00,0x00,0x08,0x08,0x1C,0x1D,0x00,0x20,0x20,0x70,0x70,0xF8,0xF8,0xFC
,0x00,0x00,0x3F,0x20,0x2C,0x2C,0x2D,0x2D,0x00,0x00,0xFC,0x04,0x04,0x04,0x84,0x84
,0xFF,0x80,0xBF,0xA0,0xAA,0xA5,0xAA,0xA5,0xFF,0x01,0xFD,0x05,0xA5,0x55,0xA5,0x55
,0x00,0x78,0x4A,0x7B,0x07,0x37,0x30,0x00,0x00,0x1E,0x52,0xDE,0xE0,0xEC,0x0C,0x00
,0x50,0x5F,0x50,0x57,0x57,0x50,0x5F,0x00,0x0A,0xDA,0x0A,0xEA,0xEA,0x0A,0xFA,0x00
,0xB0,0xB0,0xB0,0xBF,0xA0,0x80,0xFF,0x80,0x12,0x12,0x12,0xF2,0x02,0x02,0xFE,0x00
,0x03,0x00,0x4F,0x1F,0x1F,0x0F,0x00,0x00,0xF8,0x00,0x9A,0xD8,0xC0,0x80,0x00,0x00
,0x46,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0xD8,0xDA,0xDA,0x1A,0x1A,0x02,0xFE,0x00
,0x3E,0x3E,0x7F,0x7F,0x3E,0x00,0x08,0x00,0xFC,0xFE,0x7E,0x7C,0x00,0x20,0x00,0x00
,0x2D,0x2D,0x2D,0x2D,0x20,0x3F,0x00,0x00,0xB4,0xB4,0xB4,0xB4,0x04,0xFC,0x00,0x00
,0xAA,0xA5,0xAA,0xA5,0xA0,0xBF,0x80,0xFF,0xA5,0x55,0xA5,0x55,0x05,0xFD,0x01,0xFF
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
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00/*
  
  
  0x2a,0x1e,0x95,0x33,0x27,0x9f,0x31,0x25,0x8a,0x2f,0x23,0x8b,0x2e,0x22,
  0x89,0x2c,0x20,0x8b,0x2a,0x1e,0x95,0x28,0x1c,0x8a,0x27,0x1b,0x95,0x25,
  0x19,0x8b,0x23,0x17,0x89,0x22,0x12,0x8b,0x23,0x8a,0x25,0x8b,0x27,0x89,
  0x28,0x1e,0x8c,0x2a,0x1c,0x89,0x2c,0x1b,0x8c,0x2e,0x19,0x89,0x2f,0x17,
  0x95,0x27,0x23,0x1e,0x95,0x12,0x95,0x23,0x1e,0x27,0x95,0x2f,0x33,0x17,
  0x95,0x2f,0x33,0x27,0x94,0x12,0x95,0x33,0x36,0x23,0x95,0x38,0x32,0x17,
  0x8b,0x33,0x36,0x89,0x1e,0x27,0x23,0x8b,0x38,0x32,0x8a,0x33,0x36,0x12,
  0x8b,0x32,0x38,0x89,0x36,0x33,0x27,0x95,0x3d,0x37,0x10,0x8b,0x3b,0x38,
  0x8a,0x20,0x23,0x28,0x8b,0x37,0x3d,0x89,0x38,0x3b,0x10,0x8b,0x37,0x3d,
  0x8a,0x3b,0x38,0x28,0x8b,0x36,0x33,0x8a,0x17,0x95,0x2a,0x1e,0x23,0x8b,
  0x2c,0x89,0x2e,0x12,0x8b,0x2f,0x8a,0x31,0x27,0x23,0x8b,0x32,0x89,0x2f,
  0x33,0x17,0x95,0x33,0x2f,0x1e,0x95,0x12,0x94,0x36,0x33,0x1e,0x95,0x32,
  0x38,0x17,0x8b,0x36,0x33,0x89,0x23,0x1e,0x27,0x8b,0x38,0x32,0x8a,0x33,
  0x36,0x18,0x8b,0x33,0x36,0x8a,0x33,0x36,0x2a,0x95,0x36,0x19,0x8b,0x31,
  0x2e,0x89,0x25,0x22,0x2a,0x8b,0x36,0x8a,0x2f,0x35,0x19,0x8b,0x36,0x89,
  0x38,0x2f,0x29,0x95,0x2e,0x36,0x1e,0x95,0x1c,0x28,0x95,0x27,0x1b,0x8b,
  0x2a,0x8a,0x2e,0x25,0x19,0x8b,0x34,0x89,0x33,0x2f,0x23,0x95,0x33,0x2f,
  0x1e,0x95,0x12,0x94,0x33,0x36,0x23,0x95,0x32,0x38,0x17,0x8b,0x33,0x36,
  0x8a,0x23,0x27,0x1e,0x8b,0x38,0x32,0x89,0x36,0x33,0x12,0x8b,0x32,0x38,
  0x8a,0x33,0x36,0x1e,0x94,0x37,0x3d,0x10,0x8b,0x3b,0x38,0x8a,0x23,0x28,
  0x20,0x8b,0x3d,0x37,0x89,0x3b,0x38,0x1c,0x8b,0x38,0x3b,0x8a,0x3a,0x3d,
  0x38,0x8b,0x3f,0x37,0x3a,0x89,0x27,0x1b,0x8b,0x33,0x8a,0x37,0x22,0x16,
  0x8b,0x3a,0x8a,0x3f,0x0f,0x1b,0x95,0x3a,0x8b,0x3b,0x8a,0x3d,0x37,0x1c,
  0x8b,0x3b,0x38,0x89,0x23,0x28,0x20,0x8b,0x3d,0x37,0x8a,0x38,0x3b,0x10,
  0x8b,0x3d,0x37,0x89,0x38,0x3b,0x20,0x8b,0x36,0x33,0x8a,0x17,0x23,0x8b,
  0x38,0x34,0x8a,0x33,0x36,0x1e,0x8b,0x31,0x34,0x8a,0x2f,0x33,0x17,0x8b,
  0x36,0x33,0x8a,0x23,0x27,0x1e,0x8b,0x33,0x36,0x8a,0x36,0x30,0x12,0x8a,
  0x34,0x31,0x8a,0x1e,0x28,0x22,0x8b,0x30,0x36,0x89,0x34,0x31,0x12,0x8b,
  0x2e,0x28,0x33,0x8a,0x31,0x28,0x2e,0x95,0x27,0x2f,0x1e,0x95,0x12,0x95,
  0x14,0x94,0x16,0x95,0x2f,0x33,0x17,0x95,0x2f,0x33,0x27,0x94,0x12,0x95,
  0x36,0x33,0x23,0x95,0x32,0x38,0x17,0x8a,0x33,0x36,0x8a,0x23,0x27,0x1e,
  0x8b,0x32,0x38,0x8a,0x33,0x36,0x12,0x8b,0x38,0x32,0x8a,0x36,0x33,0x27,
  0x95,0x3d,0x37,0x10,0x8a,0x3b,0x38,0x8a,0x23,0x28,0x20,0x8b,0x37,0x3d,
  0x89,0x3b,0x38,0x10,0x8b,0x37,0x3d,0x8a,0x38,0x3b,0x23,0x8b,0x36,0x33,
  0x8a,0x17,0x95,0x2a,0x1e,0x23,0x8b,0x2c,0x8a,0x2e,0x12,0x8b,0x2f,0x89,
  0x31,0x23,0x1e,0x8b,0x32,0x8a,0x33,0x2f,0x17,0x95,0x2f,0x33,0x1e,0x94,
  0x12,0x95,0x36,0x33,0x27,0x95,0x38,0x32,0x17,0x8a,0x36,0x33,0x8a,0x23,
  0x1e,0x27,0x8b,0x38,0x32,0x89,0x33,0x36,0x18,0x8c,0x36,0x33,0x89,0x33,
  0x36,0x21,0x95,0x36,0x19,0x8b,0x2e,0x31,0x8a,0x25,0x2a,0x22,0x8b,0x36,
  0x89,0x35,0x2f,0x19,0x8b,0x36,0x8a,0x2f,0x38,0x25,0x94,0x36,0x2e,0x2a,
  0x95,0x1c,0x28,0x95,0x27,0x1b,0x8c,0x2a,0x89,0x2e,0x25,0x19,0x8b,0x34,
  0x8a,0x33,0x2f,0x23,0x95,0x2f,0x33,0x1e,0x94,0x12,0x95,0x33,0x36,0x23,
  0x95,0x32,0x38,0x17,0x8a,0x33,0x36,0x8a,0x23,0x1e,0x27,0x8b,0x32,0x38,
  0x89,0x36,0x33,0x12,0x8c,0x32,0x38,0x89,0x33,0x36,0x1e,0x95,0x37,0x3d,
  0x10,0x8b,0x3b,0x38,0x8a,0x23,0x20,0x28,0x8a,0x3d,0x37,0x8a,0x38,0x3b,
  0x1c,0x8b,0x3b,0x38,0x8a,0x3d,0x38,0x3a,0x8b,0x3a,0x3f,0x37,0x89,0x27,
  0x1b,0x8b,0x33,0x8a,0x37,0x22,0x16,0x8b,0x3a,0x8a,0x3f,0x0f,0x1b,0x95,
  0x3a,0x8b,0x3b,0x8a,0x3d,0x37,0x1c,0x8b,0x3b,0x38,0x8a,0x20,0x28,0x23,
  0x8b,0x3d,0x37,0x89,0x38,0x3b,0x10,0x8b,0x3d,0x37,0x8a,0x38,0x3b,0x20,
  0x8b,0x33,0x36,0x89,0x17,0x23,0x8b,0x38,0x34,0x8a,0x36,0x33,0x23,0x8b,
  0x34,0x31,0x89,0x2f,0x33,0x17,0x8c,0x36,0x33,0x89,0x1e,0x27,0x23,0x8c,
  0x33,0x36,0x89,0x36,0x30,0x12,0x8b,0x34,0x31,0x8a,0x1e,0x28,0x22,0x8a,
  0x30,0x36,0x8a,0x34,0x31,0x1e,0x8b,0x28,0x2e,0x33,0x89,0x28,0x31,0x2e,
  0x95,0x27,0x2f,0x17,0x95,0x12,0x1e,0x95,0x3b,0x36,0x33,0x96,0x2a,0x8b,
  0x2b,0x8a,0x2c,0x1e,0x12,0x8b,0x2d,0x89,0x2e,0x28,0x1e,0x8b,0x2e,0x31,
  0x8a,0x16,0x22,0x8b,0x36,0x89,0x34,0x1e,0x22,0x8b,0x31,0x8a,0x2c,0x25,
  0x19,0x8b,0x2d,0x89,0x2e,0x28,0x25,0x8b,0x33,0x2e,0x8a,0x25,0x19,0x8c,
  0x31,0x89,0x2c,0x1a,0x26,0x8b,0x2e,0x8a,0x2f,0x1b,0x27,0x8b,0x2a,0x89,
  0x2c,0x23,0x1e,0x8b,0x2e,0x8a,0x2f,0x12,0x1e,0x8b,0x30,0x89,0x31,0x27,
  0x23,0x8b,0x32,0x8a,0x33,0x17,0x23,0x8b,0x32,0x8a,0x33,0x1e,0x23,0x8b,
  0x38,0x33,0x8a,0x1e,0x12,0x8b,0x36,0x8a,0x31,0x1e,0x23,0x8b,0x33,0x89,
  0x34,0x25,0x19,0x8b,0x3d,0x89,0x1e,0x28,0x22,0x8b,0x33,0x89,0x34,0x12,
  0x1e,0x8b,0x3d,0x8a,0x1e,0x22,0x28,0x8a,0x33,0x8a,0x34,0x19,0x25,0x8b,
  0x3d,0x89,0x28,0x1e,0x22,0x8b,0x3b,0x89,0x3a,0x1e,0x12,0x8b,0x38,0x8a,
  0x36,0x22,0x16,0x8b,0x34,0x8a,0x33,0x23,0x17,0x8b,0x3b,0x8a,0x1e,0x23,
  0x27,0x8a,0x32,0x8a,0x33,0x12,0x1e,0x8b,0x3b,0x89,0x27,0x23,0x1e,0x8b,
  0x32,0x89,0x33,0x23,0x17,0x8b,0x3b,0x89,0x1e,0x23,0x27,0x8b,0x38,0x8a,
  0x36,0x12,0x1e,0x8b,0x33,0x89,0x31,0x1e,0x27,0x8b,0x2f,0x8a,0x2e,0x20,
  0x14,0x8b,0x2f,0x8a,0x30,0x2a,0x24,0x8b,0x30,0x38,0x8a,0x24,0x18,0x8b,
  0x36,0x89,0x33,0x24,0x20,0x8b,0x30,0x8a,0x2e,0x27,0x1b,0x8b,0x2f,0x89,
  0x30,0x24,0x2a,0x8b,0x30,0x38,0x8a,0x20,0x14,0x8b,0x33,0x36,0x8a,0x38,
  0x30,0x24,0x8b,0x33,0x36,0x89,0x34,0x31,0x19,0x8b,0x30,0x33,0x89,0x31,
  0x34,0x25,0x8b,0x2c,0x89,0x1c,0x8b,0x30,0x8a,0x31,0x20,0x28,0x8a,0x34,
  0x8a,0x38,0x19,0x8b,0x33,0x89,0x34,0x25,0x20,0x8b,0x31,0x8a,0x25,0x20,
  0x1c,0x8b,0x2c,0x8a,0x28,0x8b,0x25,0x8a,0x26,0x1d,0x8a,0x29,0x20,0x8a,
  0x2c,0x23,0x8b,0x2f,0x26,0x89,0x32,0x29,0x8b,0x32,0x29,0x94,0x32,0x29,
  0x94,0x35,0x2c,0x8a,0x38,0x2f,0x8a,0x3b,0x32,0x8a,0x35,0x3e,0xa8,0x2f,
  0x3f,0x36,0x8b,0x3b,0x8a,0x36,0x8b,0x33,0x94,0x2f,0x8a,0x27,0x8b,0x2a,
  0x89,0x2a,0x2e,0x28,0x8b,0x31,0x2a,0x2e,0x8a,0x1e,0x12,0x8b,0x2f,0x2a,
  0x27,0x8a,0x17,0x23,0x95,0x2a,0x8b,0x2b,0x8a,0x2c,0x1e,0x12,0x8a,0x2d,
  0x8a,0x2e,0x25,0x1e,0x8b,0x31,0x2e,0x89,0x22,0x16,0x8b,0x36,0x8a,0x34,
  0x1e,0x22,0x8a,0x31,0x8a,0x2c,0x25,0x19,0x8b,0x2d,0x89,0x2e,0x28,0x25,
  0x8b,0x33,0x2e,0x8a,0x19,0x25,0x8a,0x31,0x8a,0x2c,0x1a,0x26,0x8b,0x2e,
  0x89,0x2f,0x27,0x1b,0x8b,0x2a,0x89,0x2c,0x23,0x1e,0x8b,0x2e,0x8a,0x2f,
  0x1e,0x12,0x8b,0x30,0x89,0x31,0x27,0x23,0x8b,0x32,0x8a,0x33,0x17,0x23,
  0x8b,0x32,0x8a,0x33,0x1e,0x23,0x8b,0x33,0x38,0x8a,0x12,0x1e,0x8b,0x36,
  0x8a,0x31,0x27,0x1e,0x8b,0x33,0x89,0x34,0x25,0x19,0x8b,0x3d,0x89,0x28,
  0x1e,0x22,0x8b,0x33,0x8a,0x34,0x1e,0x12,0x8b,0x3d,0x89,0x1e,0x22,0x28,
  0x8b,0x33,0x89,0x34,0x19,0x25,0x8b,0x3d,0x8a,0x22,0x1e,0x28,0x8b,0x3b,
  0x89,0x3a,0x12,0x1e,0x8b,0x38,0x89,0x36,0x22,0x16,0x8b,0x34,0x8a,0x33,
  0x23,0x17,0x8b,0x3b,0x8a,0x1e,0x27,0x23,0x8b,0x32,0x8a,0x33,0x1e,0x12,
  0x8a,0x3b,0x8a,0x23,0x1e,0x27,0x8b,0x32,0x89,0x33,0x23,0x17,0x8b,0x3b,
  0x8a,0x27,0x1e,0x23,0x8a,0x38,0x8a,0x36,0x12,0x1e,0x8b,0x33,0x8a,0x31,
  0x1e,0x23,0x8b,0x2f,0x89,0x2e,0x14,0x20,0x8b,0x2f,0x8a,0x30,0x24,0x20,
  0x8b,0x38,0x30,0x89,0x18,0x24,0x8c,0x36,0x8a,0x33,0x24,0x20,0x8a,0x30,
  0x8a,0x2e,0x27,0x1b,0x8b,0x2f,0x89,0x30,0x24,0x20,0x8b,0x38,0x30,0x8a,
  0x14,0x20,0x8b,0x36,0x33,0x8a,0x30,0x38,0x20,0x8b,0x36,0x33,0x8a,0x31,
  0x34,0x19,0x8b,0x33,0x30,0x89,0x34,0x31,0x25,0x8b,0x2c,0x8a,0x1c,0x8a,
  0x30,0x8a,0x31,0x28,0x20,0x8b,0x34,0x89,0x38,0x19,0x8b,0x33,0x8a,0x34,
  0x25,0x20,0x8b,0x31,0x8a,0x25,0x20,0x1c,0x8b,0x2c,0x89,0x28,0x8b,0x25,
  0x8a,0x26,0x1d,0x8b,0x29,0x20,0x89,0x2c,0x23,0x8b,0x2f,0x26,0x8a,0x32,
  0x29,0x8a,0x32,0x29,0x95,0x32,0x29,0x94,0x35,0x2c,0x8a,0x38,0x2f,0x8b,
  0x3b,0x32,0x89,0x3e,0x35,0xa9,0x36,0x3f,0x2f,0x8b,0x3b,0x8a,0x36,0x8b,
  0x33,0x95,0x2f,0x89,0x27,0x8b,0x2a,0x89,0x28,0x31,0x2a,0x8b,0x2e,0x31,
  0x28,0x8a,0x12,0x1e,0x8b,0x27,0x2a,0x2f,0x8a,0x17,0x23,0x95,0x2a,0x94,
  0x33,0x2f,0x17,0x94,0x2f,0x33,0x1e,0x95,0x12,0x94,0x33,0x36,0x23,0x95,
  0x38,0x32,0x17,0x8a,0x33,0x36,0x8a,0x27,0x1e,0x23,0x8b,0x32,0x38,0x8a,
  0x33,0x36,0x12,0x8b,0x32,0x38,0x89,0x33,0x36,0x27,0x95,0x3d,0x37,0x10,
  0x8b,0x38,0x3b,0x8a,0x28,0x20,0x23,0x8b,0x3d,0x37,0x89,0x3b,0x38,0x10,
  0x8b,0x3d,0x37,0x8a,0x38,0x3b,0x23,0x8b,0x36,0x33,0x8a,0x17,0x95,0x2a,
  0x1e,0x23,0x8b,0x2c,0x89,0x2e,0x12,0x8b,0x2f,0x8a,0x31,0x23,0x1e,0x8b,
  0x32,0x89,0x33,0x2f,0x17,0x94,0x2f,0x33,0x1e,0x95,0x12,0x94,0x33,0x36,
  0x27,0x95,0x38,0x32,0x17,0x8b,0x36,0x33,0x8a,0x23,0x27,0x1e,0x8b,0x32,
  0x38,0x8a,0x33,0x36,0x18,0x8b,0x36,0x33,0x8a,0x33,0x36,0x2a,0x95,0x36,
  0x19,0x8b,0x31,0x2e,0x89,0x22,0x2a,0x25,0x8b,0x36,0x89,0x35,0x2f,0x19,
  0x8b,0x36,0x8a,0x2f,0x38,0x25,0x95,0x2e,0x36,0x2a,0x95,0x28,0x1c,0x95,
  0x27,0x1b,0x8b,0x2a,0x8a,0x2e,0x25,0x19,0x8b,0x34,0x89,0x33,0x2f,0x17,
  0x95,0x2f,0x33,0x23,0x94,0x12,0x95,0x33,0x36,0x1e,0x95,0x38,0x32,0x17,
  0x8b,0x36,0x33,0x8a,0x23,0x27,0x1e,0x8b,0x32,0x38,0x8a,0x33,0x36,0x12,
  0x8b,0x38,0x32,0x8a,0x33,0x36,0x1e,0x95,0x3d,0x37,0x10,0x8b,0x3b,0x38,
  0x89,0x23,0x28,0x20,0x8b,0x37,0x3d,0x8a,0x38,0x3b,0x1c,0x8b,0x38,0x3b,
  0x89,0x38,0x3d,0x3a,0x8b,0x3f,0x3a,0x37,0x8a,0x1b,0x27,0x8b,0x33,0x8a,
  0x37,0x22,0x16,0x8b,0x3a,0x8a,0x3f,0x0f,0x1b,0x95,0x3a,0x8b,0x3b,0x89,
  0x3d,0x37,0x1c,0x8b,0x38,0x3b,0x8a,0x28,0x20,0x23,0x8a,0x3d,0x37,0x8a,
  0x38,0x3b,0x10,0x8b,0x3d,0x37,0x89,0x3b,0x38,0x20,0x8b,0x33,0x36,0x8a,
  0x23,0x17,0x8b,0x38,0x34,0x8a,0x33,0x36,0x27,0x8b,0x34,0x31,0x8a,0x33,
  0x2f,0x23,0x8b,0x36,0x33,0x8a,0x1e,0x23,0x27,0x8b,0x33,0x36,0x8a,0x36,
  0x30,0x12,0x8b,0x34,0x31,0x89,0x1e,0x28,0x22,0x8b,0x30,0x36,0x89,0x31,
  0x34,0x1e,0x8b,0x2e,0x28,0x33,0x8a,0x28,0x31,0x2e,0x95,0x2f,0x27,0x17,
  0x95,0x12,0x1e,0x95,0x36,0x33,0x3b,0xa9,0x34,0x28,0x95,0x34,0x28,0xa0,
  0x2f,0x23,0x8a,0x34,0x28,0x8b,0x36,0x2a,0x8a,0x38,0x2c,0x94,0x2c,0x38,
  0xa1,0x2f,0x23,0x89,0x28,0x34,0x8b,0x2c,0x38,0x8a,0x39,0x3b,0x2f,0x8b,
  0x3f,0x39,0x36,0x94,0x33,0x39,0x3d,0x8a,0x17,0x23,0x8b,0x3b,0x33,0x39,
  0x89,0x33,0x2f,0x2d,0x95,0x34,0x2c,0x2f,0x95,0x1c,0x10,0x95,0x12,0x1e,
  0x95,0x2f,0x14,0x20,0x8b,0x30,0x89,0x31,0x21,0x15,0x8b,0x39,0x8a,0x28,
  0x2d,0x25,0x8b,0x36,0x8a,0x30,0x1e,0x2a,0x8b,0x39,0x89,0x1f,0x2b,0x8b,
  0x36,0x8a,0x2f,0x2c,0x20,0x8b,0x34,0x8a,0x38,0x23,0x28,0x8b,0x3d,0x89,
  0x1c,0x28,0x8b,0x3b,0x8a,0x38,0x23,0x28,0x8b,0x34,0x8a,0x33,0x17,0x23,
  0x8b,0x3b,0x89,0x36,0x2d,0x23,0x8b,0x33,0x8a,0x31,0x1e,0x2a,0x8b,0x33,
  0x89,0x1f,0x2b,0x8c,0x2f,0x89,0x34,0x20,0x2c,0x8b,0x34,0x8a,0x38,0x28,
  0x1c,0x8b,0x3b,0x89,0x40,0x23,0x17,0x8b,0x3d,0x8a,0x3b,0x14,0x20,0x8b,
  0x38,0x8a,0x31,0x15,0x21,0x8b,0x39,0x89,0x28,0x25,0x2d,0x8b,0x36,0x8a,
  0x30,0x1e,0x2a,0x8b,0x39,0x8a,0x2b,0x1f,0x8b,0x36,0x89,0x2f,0x2c,0x20,
  0x8b,0x34,0x8a,0x38,0x28,0x2c,0x8b,0x3d,0x8a,0x28,0x1c,0x8b,0x3b,0x89,
  0x38,0x28,0x2c,0x8b,0x34,0x8a,0x36,0x19,0x25,0x8b,0x38,0x8a,0x36,0x28,
  0x2e,0x8b,0x34,0x89,0x33,0x12,0x1e,0x8b,0x34,0x8a,0x2a,0x2e,0x28,0x8b,
  0x31,0x89,0x2f,0x27,0x2a,0x81,0x23,0x94,0x32,0x38,0x1d,0x8b,0x33,0x36,
  0x1e,0x8a,0x33,0x3b,0x17,0x95,0x2f,0x32,0x20,0x94,0x31,0x15,0x21,0x8b,
  0x39,0x8a,0x2d,0x28,0x25,0x8b,0x36,0x8a,0x30,0x1e,0x2a,0x8b,0x39,0x8a,
  0x1f,0x2b,0x8b,0x36,0x89,0x20,0x2c,0x8b,0x2f,0x8a,0x34,0x23,0x2c,0x8b,
  0x38,0x8a,0x3d,0x28,0x1c,0x8b,0x3b,0x8a,0x38,0x2c,0x28,0x8b,0x34,0x89,
  0x33,0x17,0x23,0x8b,0x3b,0x8a,0x36,0x23,0x2d,0x8b,0x33,0x8a,0x31,0x1e,
  0x2a,0x8b,0x33,0x8a,0x2b,0x1f,0x8b,0x2f,0x89,0x2c,0x20,0x8b,0x34,0x8a,
  0x38,0x28,0x1c,0x8b,0x3b,0x8a,0x38,0x40,0x17,0x8b,0x3d,0x8a,0x32,0x3b,
  0x20,0x8b,0x38,0x89,0x31,0x15,0x21,0x8b,0x39,0x8a,0x25,0x19,0x8b,0x36,
  0x8a,0x30,0x2a,0x1e,0x8b,0x39,0x8a,0x21,0x2d,0x8b,0x36,0x89,0x2c,0x20,
  0x8b,0x2f,0x8a,0x34,0x28,0x1c,0x8b,0x38,0x89,0x3d,0x23,0x17,0x8b,0x3b,
  0x8a,0x38,0x28,0x1c,0x8b,0x2f,0x89,0x2e,0x19,0x25,0x8b,0x31,0x34,0x8a,
  0x12,0x1e,0x95,0x2d,0x0b,0x17,0x8b,0x33,0x36,0x95,0x34,0x2c,0x8a,0x1c,
  0x10,0x96,0x38,0x1c,0x28,0x8b,0x34,0x89,0x36,0x23,0x17,0x8b,0x38,0x8b,
  0x2f,0x14,0x20,0x8b,0x30,0x89,0x31,0x21,0x15,0x8b,0x39,0x8a,0x28,0x25,
  0x2d,0x8b,0x36,0x89,0x30,0x2a,0x1e,0x8b,0x39,0x8a,0x1f,0x2b,0x8b,0x36,
  0x8a,0x2f,0x2c,0x20,0x8b,0x34,0x89,0x38,0x23,0x28,0x8b,0x3d,0x8a,0x1c,
  0x28,0x8b,0x3b,0x8a,0x38,0x23,0x28,0x8b,0x34,0x89,0x33,0x17,0x23,0x8b,
  0x3b,0x8a,0x36,0x2d,0x27,0x8b,0x33,0x8a,0x31,0x2a,0x1e,0x8b,0x33,0x89,
  0x2b,0x1f,0x8b,0x2f,0x8a,0x34,0x20,0x2c,0x8b,0x34,0x8a,0x38,0x28,0x1c,
  0x8b,0x3b,0x8a,0x40,0x17,0x23,0x8b,0x3d,0x89,0x3b,0x20,0x14,0x8c,0x38,
  0x89,0x31,0x15,0x21,0x8b,0x39,0x8a,0x2d,0x25,0x28,0x8b,0x36,0x8a,0x30,
  0x1e,0x2a,0x8b,0x39,0x8a,0x2b,0x1f,0x8b,0x36,0x8a,0x2f,0x2c,0x20,0x8b,
  0x34,0x8a,0x38,0x23,0x28,0x8b,0x3d,0x8a,0x28,0x1c,0x8b,0x3b,0x89,0x38,
  0x23,0x28,0x8c,0x34,0x89,0x36,0x19,0x25,0x8b,0x38,0x8a,0x36,0x28,0x2e,
  0x8b,0x34,0x89,0x33,0x12,0x1e,0x8c,0x34,0x89,0x2e,0x28,0x2a,0x8c,0x31,
  0x89,0x2f,0x2a,0x27,0x81,0x23,0x94,0x32,0x38,0x1d,0x8c,0x33,0x36,0x1e,
  0x89,0x3b,0x33,0x17,0x95,0x2f,0x32,0x20,0x95,0x31,0x21,0x15,0x8b,0x39,
  0x8a,0x2d,0x28,0x25,0x8b,0x36,0x8a,0x30,0x2a,0x1e,0x8b,0x39,0x8a,0x1f,
  0x2b,0x8b,0x36,0x8a,0x2c,0x20,0x8b,0x2f,0x89,0x34,0x23,0x28,0x8c,0x38,
  0x89,0x3d,0x28,0x1c,0x8b,0x3b,0x8a,0x38,0x28,0x23,0x8b,0x34,0x8a,0x33,
  0x17,0x23,0x8b,0x3b,0x8a,0x36,0x2d,0x27,0x8b,0x33,0x8a,0x31,0x2a,0x1e,
  0x8b,0x33,0x8a,0x2b,0x1f,0x8b,0x2f,0x89,0x2c,0x20,0x8c,0x34,0x89,0x38,
  0x1c,0x28,0x8b,0x3b,0x8a,0x40,0x38,0x23,0x8b,0x3d,0x8a,0x32,0x3b,0x14,
  0x8b,0x38,0x8a,0x31,0x15,0x21,0x8b,0x39,0x8a,0x19,0x25,0x8b,0x36,0x8a,
  0x30,0x2a,0x1e,0x8b,0x39,0x8a,0x2d,0x21,0x8b,0x36,0x8a,0x2c,0x20,0x8b,
  0x2f,0x89,0x34,0x1c,0x28,0x8b,0x38,0x8a,0x3d,0x23,0x17,0x8b,0x3b,0x89,
  0x38,0x1c,0x28,0x8c,0x2f,0x89,0x2e,0x19,0x25,0x8b,0x31,0x34,0x8a,0x12,
  0x1e,0x95,0x2d,0x0b,0x17,0x8c,0x33,0x36,0x95,0x2c,0x34,0x8a,0x1c,0x10,
  0x95,0x17,0x96,0x10,0x8b,0x3b,0x2f,0x8a,0x31,0x3d,0x8b,0x3e,0x32,0x8a,
  0x3f,0x33,0x39,0x94,0x3b,0x2f,0x23,0x8b,0x39,0x3d,0x31,0x8a,0x23,0x17,
  0x95,0x2d,0x23,0x27,0x95,0x3f,0x39,0x33,0x94,0x3b,0x2f,0x2d,0x8b,0x3d,
  0x39,0x31,0x8a,0x12,0x1e,0x8b,0x3b,0x2f,0x17,0x8a,0x31,0x3d,0x22,0x8b,
  0x3f,0x33,0x15,0x8a,0x38,0x40,0x34,0x95,0x38,0x3b,0x2f,0x8b,0x31,0x38,
  0x3d,0x89,0x1c,0x28,0x95,0x28,0x2c,0x23,0x95,0x34,0x40,0x38,0x95,0x38,
  0x2f,0x3b,0x8b,0x31,0x3d,0x38,0x89,0x28,0x1c,0x95,0x29,0x1d,0x95,0x1e,
  0x2a,0x95,0x3d,0x27,0x23,0x8b,0x39,0x3b,0x33,0x89,0x23,0x17,0x95,0x2d,
  0x27,0x23,0x95,0x2a,0x1e,0x95,0x3d,0x23,0x27,0x8b,0x33,0x3b,0x39,0x89,
  0x23,0x17,0x95,0x27,0x23,0x2d,0x95,0x1c,0x28,0x95,0x3d,0x28,0x2c,0x8b,
  0x34,0x3b,0x38,0x89,0x23,0x17,0x95,0x28,0x23,0x2c,0x95,0x1c,0x28,0x95,
  0x3d,0x28,0x23,0x8b,0x38,0x34,0x3b,0x8a,0x20,0x2c,0x8b,0x2f,0x3b,0x89,
  0x3d,0x31,0x1f,0x8b,0x32,0x3e,0x8a,0x3f,0x39,0x33,0x95,0x3b,0x2f,0x27,
  0x8b,0x39,0x31,0x3d,0x8a,0x23,0x17,0x94,0x27,0x2d,0x23,0x95,0x33,0x39,
  0x3f,0x95,0x3b,0x2f,0x27,0x8b,0x3d,0x31,0x39,0x8a,0x12,0x1e,0x8b,0x3b,
  0x2f,0x23,0x89,0x3d,0x31,0x22,0x8b,0x33,0x3f,0x15,0x8a,0x34,0x40,0x38,
  0x95,0x3b,0x38,0x2f,0x8b,0x3d,0x31,0x38,0x8a,0x1c,0x28,0x95,0x28,0x23,
  0x2c,0x95,0x20,0x2c,0x8c,0x34,0x8a,0x38,0x28,0x1c,0x8b,0x3b,0x8a,0x40,
  0x38,0x23,0x8b,0x3d,0x8a,0x32,0x3b,0x20,0x8b,0x38,0x8a,0x31,0x15,0x21,
  0x8b,0x39,0x8a,0x19,0x25,0x8b,0x36,0x8a,0x30,0x1e,0x2a,0x8b,0x39,0x8a,
  0x2d,0x21,0x8b,0x36,0x8a,0x2c,0x20,0x8b,0x2f,0x8a,0x34,0x28,0x1c,0x8b,
  0x38,0x89,0x3d,0x17,0x23,0x8b,0x3b,0x8a,0x38,0x1c,0x28,0x8b,0x2f,0x8a,
  0x2e,0x19,0x25,0x8b,0x34,0x31,0x89,0x1e,0x12,0x95,0x2d,0x17,0x0b,0x8b,
  0x36,0x33,0x95,0x2c,0x34,0x8a,0x10,0x1c,0x95,0x17,0x23,0x96,0x20,0x2c,
  0x8b,0x2f,0x3b,0x89,0x31,0x3d,0x2b,0x8b,0x32,0x3e,0x89,0x39,0x3f,0x33,
  0x95,0x2f,0x3b,0x23,0x8b,0x39,0x31,0x3d,0x8a,0x23,0x17,0x95,0x2d,0x23,
  0x27,0x95,0x3f,0x39,0x33,0x95,0x2f,0x3b,0x23,0x8b,0x31,0x3d,0x39,0x89,
  0x1e,0x12,0x8c,0x2f,0x3b,0x23,0x89,0x31,0x3d,0x22,0x8b,0x3f,0x33,0x15,
  0x89,0x40,0x38,0x34,0x95,0x3b,0x2f,0x38,0x8b,0x3d,0x31,0x38,0x8a,0x1c,
  0x28,0x95,0x23,0x28,0x2c,0x95,0x38,0x34,0x40,0x95,0x3b,0x38,0x2f,0x8b,
  0x31,0x3d,0x38,0x89,0x1c,0x28,0x95,0x29,0x1d,0x95,0x2a,0x1e,0x95,0x3d,
  0x27,0x23,0x8b,0x39,0x3b,0x33,0x89,0x17,0x23,0x95,0x2d,0x27,0x23,0x95,
  0x1e,0x2a,0x95,0x3d,0x27,0x23,0x8b,0x33,0x39,0x3b,0x8a,0x23,0x17,0x94,
  0x2d,0x23,0x27,0x95,0x1c,0x28,0x95,0x3d,0x2c,0x28,0x8b,0x34,0x3b,0x38,
  0x89,0x17,0x23,0x95,0x2c,0x28,0x23,0x95,0x1c,0x28,0x94,0x3d,0x23,0x2c,
  0x8b,0x38,0x34,0x3b,0x8a,0x2c,0x20,0x8b,0x2f,0x3b,0x89,0x31,0x3d,0x1f,
  0x8b,0x32,0x3e,0x8a,0x3f,0x39,0x33,0x94,0x3b,0x2f,0x2d,0x8b,0x31,0x39,
  0x3d,0x8a,0x23,0x17,0x95,0x2d,0x27,0x23,0x95,0x39,0x3f,0x33,0x95,0x3b,
  0x2f,0x27,0x8b,0x31,0x39,0x3d,0x89,0x12,0x1e,0x8b,0x3b,0x2f,0x17,0x8a,
  0x31,0x3d,0x22,0x8b,0x33,0x3f,0x15,0x89,0x34,0x40,0x38,0x95,0x3b,0x38,
  0x2f,0x8b,0x3d,0x31,0x38,0x8a,0x28,0x1c,0x95,0x28,0x23,0x2c,0x95,0x20,
  0x2c,0x8b,0x34,0x89,0x38,0x1c,0x28,0x8b,0x3b,0x8a,0x38,0x40,0x23,0x8b,
  0x3d,0x8a,0x3b,0x32,0x20,0x8b,0x38,0x89,0x31,0x15,0x21,0x8b,0x39,0x8a,
  0x19,0x25,0x8b,0x36,0x8a,0x30,0x1e,0x2a,0x8b,0x39,0x89,0x2d,0x21,0x8b,
  0x36,0x8a,0x20,0x2c,0x8b,0x2f,0x8a,0x34,0x28,0x1c,0x8a,0x38,0x8a,0x3d,
  0x23,0x17,0x8b,0x3b,0x8a,0x38,0x28,0x1c,0x8b,0x2f,0x89,0x2e,0x25,0x19,
  0x8c,0x31,0x34,0x89,0x1e,0x12,0x95,0x2d,0x17,0x0b,0x8c,0x33,0x36,0x95,
  0x34,0x2c,0x8a,0x1c,0x10,0x95,0x17,0x96,0x40,0x34,0x3b,0xff*/};

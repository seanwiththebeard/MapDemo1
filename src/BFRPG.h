#include "Common.h"

extern struct
{
  int
  	CARRYING_WEIGHT,
  	CARRYING_CAPACITY,
  	GOLD;
}Party;

extern struct
{
  char
    	NAME[16];
  byte
    	HITDICEMAX;
}RaceDescription[];

extern struct
{
  char
    	NAME[16];
  byte
    	HITDICE;
}ClassDescription[];

extern int AbilityModifier[];
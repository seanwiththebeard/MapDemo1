#include "Common.h"

typedef struct
{  
  byte
    	HP,
  	HPMAX,
  	STR,
  	DEX,
        CON,
        INT,
  	WIS,
        CHR,
  
  	RACE,
  	CLASS,
  	HITDICE,
  	LEVEL;
  int
    	EXP,
  	CARRYING_WEIGHT,
  	CARRYING_CAPACITY;
  char
    	NAME[16];
}playerChar;

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
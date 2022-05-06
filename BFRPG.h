#include "Common.h"

extern struct 
{
  char
    	NAME[16];
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
  
  	LEVEL_FIGHTER,
  	LEVEL_THIEF,
  	LEVEL_CLERIC,
  	LEVEL_WIZARD;
  int
    	EXP,
  	CARRYING_WEIGHT,
  	CARRYING_CAPACITY,
  	GOLD;
}playerChar[4];

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
    	HITDICE;
}RaceDescription[4];

extern struct
{
  char
    	NAME[16];
  byte
    	HITDICE;
}ClassDescription[4];
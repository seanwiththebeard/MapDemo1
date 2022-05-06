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
  	HITDICE,
  
  	LEVEL_FIGHTER,
  	LEVEL_THIEF,
  	LEVEL_CLERIC,
  	LEVEL_WIZARD;
  int
    	EXP,
  	CARRYING_WEIGHT,
  	CARRYING_CAPACITY,
  	GOLD;
}playerChar[];

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
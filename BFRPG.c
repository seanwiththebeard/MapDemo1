#include "BFRPG.h"
struct 
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
}playerChar[4];

struct
{
  int
  	CARRYING_WEIGHT,
  	CARRYING_CAPACITY,
  	GOLD;
}Party;

struct
{
  char
    	NAME[16];
  byte
    	HITDICEMAX;
}RaceDescription[4] = {{"Human@", 255},{"Elf@", 6}, {"Dwarf@", 255}, {"Halfling@", 6}};

struct
{
  char
    	NAME[16];
  byte
    	HITDICE;
}ClassDescription[4] = {{"Fighter@", 8},{"Magic-User@", 4}, {"Cleric@", 6}, {"Thief@", 4}};

int AbilityModifier[20] = {-3, -3, -3, -3, -3, -2, -2, -1, -1, -1, 0, 0, 0, 0, +1, +1, +1, +2, +2, +3};
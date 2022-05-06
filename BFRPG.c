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
    	HITDICE;
}RaceDescription[4] = {{"Human@", 6},{"Elf@", 6}, {"Dwarf@", 6}, {"Halfling@", 6}};

struct
{
  char
    	NAME[16];
  byte
    	HITDICE;
}ClassDescription[4] = {{"Fighter@", 6},{"Magic-User@", 6}, {"Cleric@", 6}, {"Thief@", 6}};
//#include "BFRPG.h"
#include "Common.h"

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
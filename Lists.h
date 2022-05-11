void create(void);
void delete_pos(byte pos);
typedef struct playerChar
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
        struct playerChar *next;
};

struct playerChar *getPlayerChar(byte index);

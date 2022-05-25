//Roster
void create(void);
void delete_pos(byte pos);
byte CountRoster(void);
typedef struct playerChar
{
  bool inParty;
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


//Party
void AddParty(byte index);
void DeleteParty(byte pos);
byte CountParty(void);

struct playerChar *getPartyMember(byte index);
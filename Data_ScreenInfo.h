#include "Common.h"
extern struct
{
    int CharIndex[4];
    byte Chars[2];
    byte ScatterIndex;
    byte NPCIndex;
    byte MusicIndex;
}ScreenData[256]={
    {
        {130, 131, 146, 147},
        32, 33,
        0, 0, 0
    },
    {
        {0, 0, 0, 0},
        0, 1,
        0, 0, 0
    } 
};
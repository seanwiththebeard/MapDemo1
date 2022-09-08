#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include "Platform.h"
//#link "SIDPlayerASM.s"
//#resource "song.sid"
#if __C64__
#include <c64.h>

void SIDINIT(void); 
void SIDPLAY(void);
void SIDSTOP(void);

void PlaySID(void)
{
  //memcpy((void*)(SIDLOAD),(void*)&SIDFILE,SIDSIZE);	//Load music into memory 
  SIDINIT(); 						//Init SID
  SIDPLAY();						//Play 	
}

void StopSID(void)
{
  SID.flt_freq = 0x00;
  SID.flt_ctrl = 0x00;
  SID.amp = 0x00;
  SIDSTOP();
}

#endif
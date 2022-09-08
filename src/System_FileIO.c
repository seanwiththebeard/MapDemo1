#if __C64__
#include <cbm.h>
#include <stdio.h>

void DiskSave(char filename[], int source, int length)
{
  //Append the save and replace prefix
  char name[16];
  sprintf(name, "@0:%s", filename);
  
  cbm_k_setlfs(0, 8, 0);
  cbm_k_setnam(name);
  cbm_k_open();
  cbm_k_save(source, source + length - 1);
  cbm_k_close(0);
  //cbm_k_clall;
}

void DiskLoad(char filename[], int dest)
{
  cbm_k_setlfs(0, 8, 0);
  cbm_k_setnam(filename);
  cbm_k_open();
  cbm_k_load(0, dest);
  cbm_k_close(0);
  //cbm_k_clall;
}

#endif
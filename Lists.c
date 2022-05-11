//Create() and Delete_Pos() based on https://www.edureka.co/blog/linked-list-in-c/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "Common.h"
#include "BFRPG.h"
#include "System_MessageWindow.h"
#include "Lists.h"
     
struct playerChar *start=NULL;

void create()
{
        struct playerChar *temp,*ptr;
        temp=(struct playerChar *)malloc(sizeof(struct playerChar));  
  
        if(temp==NULL)
          exit(0);
  
        temp->next=NULL;
        if(start==NULL)
          start=temp;
        else
        {
          ptr=start;
          while(ptr->next!=NULL)
          {
            ptr=ptr->next;
          }
          ptr->next=temp;
        }
}

struct playerChar *getPlayerChar(byte index)
{
  byte i = 0;
  struct playerChar *tmp;
  if (start == NULL)
    create();
  tmp = start;

  while (tmp != NULL)
  {
    if(i == index)
    {
      return tmp;
      tmp = tmp->next;
    }
    ++i;
  }
}

void delete_pos(byte pos)
{
        int i;
        struct playerChar *temp,*ptr;
  
        if(start==NULL)
          exit(0);
        else
        {
          if(pos==0)
          {
            ptr=start;
            start=start->next ;
          }
          else
          {
            ptr=start;
            for(i=0;i<pos;i++)
            {
              temp=ptr; 
              ptr=ptr->next ;
              if(ptr==NULL)
              {
                WriteLineMessageWindow("Position not Found:@", 0);
                return;
              }
            }
            temp->next =ptr->next ;
          }
          //sprintf(str, "Deleted element:%d",ptr->character.NAME);
          //WriteLineMessageWindow(str, 0);
          free(ptr);
        }
}
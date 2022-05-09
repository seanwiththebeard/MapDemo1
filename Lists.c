#include <stdlib.h>
#include <stdio.h>
#include "Common.h"
     
struct node
{
        int info;
        struct node *next;
};
struct node *start=NULL;

void create()
{
        struct node *temp,*ptr;
        temp=(struct node *)malloc(sizeof(struct node));
        if(temp==NULL)
        {
                //printf("nOut of Memory Space:n");
                exit(0);
        }
        //printf("nEnter the data value for the node:t");
        //scanf("%d",&temp->info);
        temp->next=NULL;
        if(start==NULL)
        {
                start=temp;
        }
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

void delete_pos(byte pos)
{
        int i;
        struct node *temp,*ptr;
        if(start==NULL)
        {
                //printf("nThe List is Empty:n");
                exit(0);
        }
        else
        {
                //printf("nEnter the position of the node to be deleted:t");
                //scanf("%d",&pos);
                if(pos==0)
                {
                        ptr=start;
                        start=start->next ;
                        //printf("nThe deleted element is:%dt",ptr->info  );
                        free(ptr);
                }
                else
                {
                        ptr=start;
                        for(i=0;i<pos;i++) { temp=ptr; ptr=ptr->next ;
                                if(ptr==NULL)
                                {
                                        //printf("nPosition not Found:n");
                                        return;
                                }
                        }
                        temp->next =ptr->next ;
                        //printf("nThe deleted element is:%dt",ptr->info );
                        free(ptr);
                }
        }
}
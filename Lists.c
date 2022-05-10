//Based on https://www.edureka.co/blog/linked-list-in-c/

#include <stdlib.h>
#include <stdio.h>
#include "Common.h"
#include "BFRPG.h"
     
struct node
{
        charPlayer character;
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
                printf("nOut of Memory Space:n");
                exit(0);
        }
        printf("Enter the data value for the node:\t");
        scanf("%d",&temp->info);
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
                printf("The List is Empty:\n");
                exit(0);
        }
        else
        {
                if(pos==0)
                {
                        ptr=start;
                        start=start->next ;
                        printf("The deleted element is:%d\t",ptr->info  );
                        free(ptr);
                }
                else
                {
                        ptr=start;
                        for(i=0;i<pos;i++) { temp=ptr; ptr=ptr->next ;
                                if(ptr==NULL)
                                {
                                        printf("Position not Found:\n");
                                        return;
                                }
                        }
                        temp->next =ptr->next ;
                        printf("The deleted element is:%d \t \n",ptr->info );
                        free(ptr);
                }
        }
}

void display()
{
        struct node *ptr;
        if(start==NULL)
        {
                printf("List is empty:\n");
                return;
        }
        else
        {
                ptr=start;
                printf("The List elements are:\n");
                while(ptr!=NULL)
                {
                        printf("%d \t ",ptr->info );
                        printf("Name:%s \t\n",ptr->character.NAME);
                        printf("STR:%d \t ",ptr->character.STR);
                        printf("DEX:%d \t ",ptr->character.DEX);
                        printf("WIS:%d \t ",ptr->character.WIS);
                        printf("INT:%d \t ",ptr->character.INT);
                  	
                        ptr=ptr->next ;
                }
                printf("\n\n");
        }
}

int ListFunction()     
{
        int choice;
  	byte pos;
        while(1){
               
                printf("MENU\n");
                printf("1.Create\n");
                printf("2.Display\n");
                printf("8.Delete from specified position \n");
                printf("9.Exit\n");
                printf("-----------\n");
                printf("Enter your choice:\t");
                scanf("%d",&choice);
                switch(choice)
                {
                        case 1:
                                        create();
                                        break;
                        case 2:
                                        display();
                                        break;
                        case 8:
                                        printf("Index to delete:\t ");
                    			
                    			scanf("%d",&pos);
                    			delete_pos(pos);
                                        break;
                        case 9:
                                        exit(0);
                                        break;
                             
                        default:
                                        printf("Wrong Choice:\n");
                                        break;
                }
        }
        return 0;
}
/**
 * @file stack.c
 * @author Petr Hýbl (xhyblp01@stud.fit.vutbr.cz)
 * @brief Implentace pomocnych funkci pro zasobnik
*/
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stack.h"
#include "expression.h"





void Stack_Init(Stack *stack)
{
	stack->top = NULL; // nastaveni inicializace
}


bool Stack_IsEmpty(Stack* stack)
{
	if (stack->top == NULL)
	{
		return true;
	}
	
	return false; //pokud je index rovny inicializaci vratim true
}




int Stack_Top_Type(Stack *stack)
{

	if (Stack_IsEmpty(stack)) //pokud je prazdny nejsou v nem data
	{
		exit(99);
	}

	return stack->top->type; // vrati vrchol zasobniku
}

char* Stack_Top_Data(Stack *stack)
{

	if (Stack_IsEmpty(stack)) //pokud je prazdny nejsou v nem data
	{
		return NULL;
	}

	

	return stack->top->data; // vrati vrchol zasobniku
}


void Stack_Pop(Stack *stack)
{

	if (!Stack_IsEmpty(stack)) //pokud neni prazdny
	{
		ptrItem *item=NULL;
		item = stack->top;
		stack->top = item->next;
		// if (item->data != NULL)
		// {
		// 	free(item->data);

		// }
		

		free(item);
	}
}


void Stack_Push(Stack *stack, int type, char *data)
{
	ptrItem *item = malloc(sizeof(ptrItem));
	if (item==NULL)
	{
		exit(99);
	}
	item->type=type;
	item->data= data;
	item->next=stack->top;
	stack->top=item;
	return;

}

void Stack_Destroy(Stack *stack)
{


	while (!Stack_IsEmpty(stack)) //pokud neni prazdny
	{
		ptrItem *item=NULL;
		item = stack->top;
		stack->top = item->next;
		free(item);
	}

	

}

int Stack_InsertBeforeNonTerm(Stack *stack, int type,char* data)
{

	ptrItem* tmp = Stack_Top_Ptr(stack);
	ptrItem* previous = NULL;
	while (tmp!= NULL)
	{
		if (tmp->type < I_HALT)
		{
			ptrItem *novy = malloc(sizeof(ptrItem));
			if (novy == NULL)
			{
				exit(99);
			}
			novy->type = type;
			novy->data = data;
			if (previous != NULL)
			{
				novy->next = previous->next;
				previous->next = novy;
			}
			else{
				novy->next = stack->top;
				stack->top = novy;
			}
			return 0;
			
			
		}
		previous = tmp;
		tmp = tmp->next;
	}
	return 0;
	


	

}

ptrItem* Stack_Top_Ptr( Stack* stack){

	if (Stack_IsEmpty(stack)) //pokud je prazdny nejsou v nem data
	{
		exit(99);
	}
	ptrItem* top = stack->top;
	return top;

}

void Stack_Print(Stack* stack){
	ptrItem* tmp = stack->top;
	printf("-------------STACK PRINT-------- ID types z expression.h \n");
	while(tmp!=NULL)
	{
		printf("%i %s \n",tmp->type,tmp->data);
		tmp = tmp->next;
	}
	printf("---konec printu-----\n");


}
/* Konec c202.c */

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




int Stack_Top(Stack *stack)
{

	if (Stack_IsEmpty(stack)) //pokud je prazdny nejsou v nem data
	{
		return -1;
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
		free(item);
	}
}


void Stack_Push(Stack *stack, int data)
{
	ptrItem *item = malloc(sizeof(ptrItem));
	if (item==NULL)
	{
		return;
	}
	item->data=data;
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

int Stack_InsertBeforeNonTerm(Stack *stack, int data)
{

	ptrItem* tmp = Stack_Top_Ptr(stack);
	ptrItem* previous = NULL;
	while (tmp!= NULL)
	{
		if (tmp->data < I_HALT)
		{
			ptrItem *novy = malloc(sizeof(ptrItem));
			if (novy == NULL)
			{
				return -1;
			}
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
		return NULL;
	}
	ptrItem* top = stack->top;
	return top;

}

void Stack_Print(Stack* stack){
	ptrItem* tmp = stack->top;
	printf("-------------STACK PRINT-------- ID types z expression.h \n");
	while(tmp!=NULL)
	{
		printf("%i \n",tmp->data);
		tmp = tmp->next;
	}
	printf("---konec printu-----\n");


}
/* Konec c202.c */

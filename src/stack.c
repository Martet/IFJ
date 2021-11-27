/**
 * @file stack.c
 * @author Petr Hýbl (xhyblp01@stud.fit.vutbr.cz)
 * @brief Implentace pomocnych funkci pro zasobnik
*/
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stack.h"






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

void Stack_InsertToTerm(Stack *stack)
{

	
	int zaloha = Stack_Top(stack);
	Stack_Pop(stack);
	zaloha = zaloha + '<';
	Stack_Push(stack,zaloha);

	

}

ptrItem* Stack_Top_Ptr( Stack* stack){

	if (Stack_IsEmpty(stack)) //pokud je prazdny nejsou v nem data
	{
		return;
	}
	ptrItem* top = stack->top;
	return top;

}

/* Konec c202.c */

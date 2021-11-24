/**
 * @file stack.c
 * @author Petr Hýbl (xhyblp01@stud.fit.vutbr.cz)
 * @brief Implentace pomocnych funkci pro zasobnik
*/

#include "stack.h"

int STACK_SIZE = MAX_STACK;
int error_flag;


void Stack_Error(int error_code)
{
	static const char *SERR_STRINGS[MAX_SERR + 1] = {
		"Unknown error",
		"Stack error: INIT",
		"Stack error: PUSH",
		"Stack error: TOP"};

	if (error_code <= 0 || error_code > MAX_SERR)
		error_code = 0;
	printf("%s\n", SERR_STRINGS[error_code]);
	error_flag = 1;
}


void Stack_Init(Stack *stack)
{

	if (stack == NULL) //pokud stack == nul vyhlasit eror
	{
		Stack_Error(SERR_INIT);
		return;
	}

	stack->topIndex = -1; // nastaveni inicializace
}


int Stack_IsEmpty(const Stack *stack)
{

	return (stack->topIndex == -1); //pokud je index rovny inicializaci vratim true
}


int Stack_IsFull(const Stack *stack)
{

	return (stack->topIndex == STACK_SIZE - 1); //pokud je index rovny maximu poctu vrati true
}


void Stack_Top(const Stack *stack, char *dataPtr)
{

	if (Stack_IsEmpty(stack)) //pokud je prazdny nejsou v nem data
	{
		Stack_Error(SERR_TOP);
	}

	*dataPtr = stack->array[stack->topIndex]; // vrati vrchol zasobniku
}


void Stack_Pop(Stack *stack)
{

	if (!Stack_IsEmpty(stack)) //pokud neni prazdny
	{
		stack->topIndex--; //vypusteni prvku na vrcholu zasobniku
	}
}


void Stack_Push(Stack *stack, char data)
{

	if (!Stack_IsFull(stack)) //pokud neni plny
	{
		stack->topIndex++;					  // zvetsi pocet prvku
		stack->array[stack->topIndex] = data; //pridej novy prvek
	}
	else
		Stack_Error(SERR_PUSH);
}

/* Konec c202.c */

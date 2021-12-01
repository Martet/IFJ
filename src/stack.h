#include <stdio.h>

#ifndef STACK_H
#define STACK_H

typedef struct item {

	struct item *next;
	int type;
	char* data;
}ptrItem;


typedef struct 
{
	ptrItem *top;
} Stack;



/**
 * @brief Inicializace zasobniku, top index == -1
 * @param stack Ukazatel na zásobník
 */
void Stack_Init( Stack* stack);

/**
 * @brief Kontrola zda je zásobník prázdný, 
 *
 * @param stack Ukazatel na zásobník
 *
 * @returns prázdný = 1, neprázdný 0
 */
bool Stack_IsEmpty( Stack* stack);

/**
 * @brief Kontrola zda je zásobnik plný
 *
 * @param stack Ukazatel na zásobník
 *
 * @returns Nenulovou hodnotu v případě, že je zásobník plný, jinak nulu
 */
int Stack_IsFull( Stack* stack );

/**
 * @brief Funkce vraci typ na vrcholu zásobniku
 *
 * @param stack Ukazatel na zásobník
 */
int Stack_Top_Type( Stack* stack);

/**
 * @brief Funkce vraci data na vrcholu
 *
 * @param stack Ukazatel na zásobník
 */
char* Stack_Top_Data( Stack* stack);

/**
 * @brief Odstraní vrcholový prvek zásobníku
 *
 * @param stack Ukazatel na zásobník
 */
void Stack_Pop( Stack* stack );

/**
 * @brief Vlozeni noveho prvku na vrchol zasobniku
 * @param stack Ukazatel na zásobník
 * @param value int k vložení
 * @param data data ulozena v tokenu
 */
void Stack_Push( Stack* stack, int value,char* data);

/**
 * @brief Zniceni zasobniku, free pameti
 * @param stack Ukazatel na zásobník
 */
void Stack_Destroy( Stack* stack);

/**
 * @brief Vraci ptrItem na vrchol zasobniku
 * @param stack Ukazatel na zásobník
 */
ptrItem* Stack_Top_Ptr( Stack* stack);

/**
 * @brief Pomocna funkce na vytiksnuti obsahu zasobniku
 * @param stack Ukazatel na zásobník
 */
void Stack_Print( Stack* stack);

/**
 * @brief Pomocna funkce ktera vlozi znak pred NON_TERM na zasobniku
 * @param stack Ukazatel na zásobník
 * @param type int k vložení
 */
int Stack_InsertBeforeNonTerm(Stack *stack, int type,char* data);

#endif
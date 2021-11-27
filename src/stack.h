#include <stdio.h>

#define MAX_STACK 20

typedef struct item {

	struct item *next;
	int data;
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
 * @brief Funkce vraci hodnotu vrcholu zásobniku
 *
 * @param stack Ukazatel na zásobník
 * @param value Ukazatel na cílovou proměnnou
 */
int Stack_Top( Stack* stack);

/**
 * @brief Odstraní vrcholový prvek zásobníku
 *
 * @param stack Ukazatel na zásobník
 */
void Stack_Pop( Stack* stack );

/**
 * @brief Vlozeni noveho prvku na vrchol zasobniku
 * @param stack Ukazatel na zásobník
 * @param data string k vložení
 */
void Stack_Push( Stack* stack, int value);

void Stack_Destroy( Stack* stack);

void Stack_InsertToTerm(Stack *stack);

ptrItem* Stack_Top_Ptr( Stack* stack);



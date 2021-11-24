#include <stdio.h>



#define MAX_STACK 20


extern int STACK_SIZE;

/** Globální proměnná - indikuje, zda operace volala chybu. */
extern int error_flag;

/** Celkový počet možných chyb. */
#define MAX_SERR    3
/** Chyba při Stack_Init. */
#define SERR_INIT   1
/** Chyba při Stack_Push. */
#define SERR_PUSH   2
/** Chyba při Stack_Top. */
#define SERR_TOP    3

/** ADT zásobník implementovaný ve statickém poli. */
typedef struct {
	/** Pole pro uložení hodnot. */
	char array[MAX_STACK];
	/** Index prvku na vrcholu zásobníku. */
	int topIndex;
} Stack;


/**
 * @brief Vytiskne chybu pokud k ni dojde
 *
 * @param eror_code typ chyby
 *
 */void Stack_Error( int );

/**
 * @brief Inicializace zasobniku, top index == -1
 * @param stack Ukazatel na zásobník
 */
void Stack_Init( Stack * );

/**
 * @brief Kontrola zda je zásobník prázdný, 
 *
 * @param stack Ukazatel na zásobník
 *
 * @returns prázdný = 1, neprázdný 0
 */
int Stack_IsEmpty( const Stack * );

/**
 * @brief Kontrola zda je zásobnik plný
 *
 * @param stack Ukazatel na zásobník
 *
 * @returns Nenulovou hodnotu v případě, že je zásobník plný, jinak nulu
 */
int Stack_IsFull( const Stack * );

/**
 * @brief Funkce vraci hodnotu vrcholu zásobniku
 *
 * @param stack Ukazatel na zásobník
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void Stack_Top( const Stack *, char * );

/**
 * @brief Odstraní vrcholový prvek zásobníku
 *
 * @param stack Ukazatel na zásobník
 */
void Stack_Pop( Stack * );

/**
 * @brief Vlozeni noveho prvku na vrchol zasobniku
 * @param stack Ukazatel na zásobník
 * @param data Znak k vložení
 */
void Stack_Push( Stack *, char );



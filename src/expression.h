/**
 * @file expression.h
 * @brief Hlavickovy soubor pro syntaktickou analyzu pro výrazy
 * @author Petr Hýbl <xhyblp01@stud.fit.vutbr.cz>
 */
#include "scanner.h"
#include "stack.h"

#ifndef EXPRESS_H
#define EXPRESS_H

typedef enum identType { //cislo za popiskem je cislo pro enum
	I_ID,  			// Identifikator 0
	I_EOL, 			// 1
	I_INTEGER, 		// 2
	I_NUMBER, 		// 3
	I_STRING, 		// 4
	I_NULL, 		// 5 
	I_PLUS,        	// '+'	scitani 6
	I_MINUS,        // '-'	odcitani 7
	I_MULTIPLAY,    // '*'	nasobeni 8
	I_DIVIDE,       // '/'	deleni 9
	I_DIVIDE_INT,   // '//'	celociselne deleni 10
	I_PAR_L,      	// '(' 11 
	I_PAR_R,      	// ')' 12
    I_DOLAR, 		// 13
	I_LESS,       	// '<' 14 
	I_LESS_EQ,    	// '<=' 15
	I_GREATER,    	// '>' 16
	I_GREATER_EQ, 	// '>=' 17
	I_ASSIGN,     	// '=' 18
	I_EQ,         	// '==' 19
	I_EQ_NIL,     	// '~=' 20
	I_CONCAT,     	// '..'21
	I_HASH,       	// '#' 22
    I_HALT,       	// <<<<<<<<< 23
    I_NON_TERM, 	// 24
    I_EROR, 		//25
} IdentType;

/**
 * @brief Funkce pro prevod TokenTypu na IdentType
 *
 * @param type typ tokenu
 * @return typ identifikatoru
 */
IdentType TokentoIden(TokenType type);

/**
 * @brief Funkce pro vyreseni vyrazu pres prediktivni tabulku vyrazu
 *
 * @param token token kterym vyraz zacina
 * @param type ulozeni typu operace
 * @return Chybovy kod
 */
int solvedExpression(token_t *token, char *type);

/**
 * @brief Funkce pro získání indexu tokenu do tabulky symbolu
 *
 * @param type typ tokenu
 * @return Index tokenu do tabulky symbolu
 */
int get_index_to_table(TokenType type);

/**
 * @brief Funkce pro získání indexu na vrcholu zasobniku pro zasobnik s ulozenymi IdentType
 *
 * @param s zasobnik s ulozenymi IdentType
 * @return Index do tabulky symbolu
 */
int stack_to_table(Stack *s);

/**
 * @brief Funkce ktera tokeny na zasobniku zmeni na NON-TERM pomoci urcenych pravidel
 *
 * @param stack zasobnik s ulozenymi IdentType
 * @param type ulozeni typu operace
 * @return kod chyby
 */
int reduce(Stack* stack, IdentType typevar, char *type);

#endif



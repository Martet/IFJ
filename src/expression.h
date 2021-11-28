/**
 * @file expression.h
 * @brief Hlavickovy soubor pro syntakticka analyza pro výrazy
 * @author Petr Hýbl <xhyblp01@stud.fit.vutbr.cz>
 */
#include "scanner.h"

#ifndef EXPRESS_H
#define EXPRESS_H

typedef enum identType {
	I_ID,  // Identifikator 0
	I_EOL, // 1
	I_INTEGER, // 2
	I_NUMBER, // 3
	I_STRING, // 4
	I_NULL, // 5 
	I_PLUS,        // '+'	scitani 3
	I_MINUS,        // '-'	odcitani 4 
	I_MULTIPLAY,        // '*'	nasobeni 5 
	I_DIVIDE,        // '/'	deleni 6 
	I_DIVIDE_INT,    // '//'	celociselne deleni 7 
	I_PAR_L,      // '(' 8 
	I_PAR_R,      // ')' 9 
    I_DOLAR, // 10
	I_LESS,       // '<' 11 
	I_LESS_EQ,    // '<=' 12
	I_GREATER,    // '>' 13
	I_GREATER_EQ, // '>=' 14
	I_ASSIGN,     // '=' 15 
	I_EQ,         // '==' 16 
	I_EQ_NIL,     // '~='17 
	I_CONCAT,     // '..'18 
	I_HASH,       // '#' 19
    I_HALT,       // <<<<<<<<< 20
    I_NON_TERM, // 21
    I_EROR,
} IdentType;

int solvedExpression(token_t *token);

int get_index_to_table(TokenType type);

#endif



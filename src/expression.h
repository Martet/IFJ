/**
 * @file expression.h
 * @brief Hlavickovy soubor pro syntakticka analyza pro výrazy
 * @author Petr Hýbl <xhyblp01@stud.fit.vutbr.cz>
 */
#include "scanner.h"

#ifndef EXPRESS_H
#define EXPRESS_H

typedef enum identType {
	I_ID,  // Identifikator
	I_EOL,
	I_PLUS,        // '+'	scitani
	I_MINUS,        // '-'	odcitani
	I_MULTIPLAY,        // '*'	nasobeni
	I_DIVIDE,        // '/'	deleni
	I_DIVIDE_INT,    // '//'	celociselne deleni
	I_PAR_L,      // '('
	I_PAR_R,      // ')'
    I_DOLAR,
	I_LESS,       // '<'
	I_LESS_EQ,    // '<='
	I_GREATER,    // '>'
	I_GREATER_EQ, // '>='
	I_ASSIGN,     // '='
	I_EQ,         // '=='
	I_EQ_NIL,     // '~='
	I_CONCAT,     // '..'
	I_HASH,       // '#'
    I_HALT,       // <<<<<<<<<
    I_NON_TERM,
    I_EROR,
} IdentType;

int solvedExpression(token_t *token);

int get_index_to_table(TokenType type);

#endif



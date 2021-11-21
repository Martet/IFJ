/**
 * @file scanner.h
 * @author Alexandr Celakovsky (xcelak00@stud.fit.vutbr.cz)
 * @brief Deklarace funkci lexikalniho analyzatoru
 */

#ifndef SCANNER_H
#define SCANNER_H

typedef enum scanState {
	START,
} ScanState;

typedef enum tokenType {
	T_ID,  // Identifikator
	T_KW,  // Klicove slovo
	T_EOF, // Konec souboru
	T_INTEGER,
	T_NUMBER,
	T_STRING,
	T_ADD,        // '+'	scitani
	T_SUB,        // '-'	odcitani
	T_MUL,        // '*'	nasobeni
	T_DIV,        // '/'	deleni
	T_DIV_INT,    // '//'	celociselne deleni
	T_COMMA,      // ','
	T_COLON,      // ':'
	T_PAR_L,      // '('
	T_PAR_R,      // ')'
	T_LESS,       // '<'
	T_LESS_EQ,    // '<='
	T_GREATER,    // '>'
	T_GREATER_EQ, // '>='
	T_EQ,         // '=='
	T_EQ_NIL,     // '~='
	T_CONCAT,     // '..'
	T_HASH,       // '#'
	T_ASSIGN,     //
} TokenType;

typedef enum tokenKeyword {
	KW_DO,
	KW_ELSE,
	KW_END,
	KW_FUNCTION,
	KW_GLOBAL,
	KW_IF,
	KW_INTEGER,
	KW_LOCAL,
	KW_NIL,
	KW_NUMBER,
	KW_REQUIRE,
	KW_RETURN,
	KW_STRING,
	KW_THEN,
	KW_WHILE
} TokenKeyword;

typedef struct token {
	TokenType type;
	TokenKeyword keyword;
	char *data;
} token_t;

/**
 * @brief Implementace pravidla <fdef_args>
 *
 * @param token Dalsi token pro zpracovani
 *
 */
int get_token(token_t *token);

#endif
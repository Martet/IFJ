/**
 * @file scanner.h
 * @author Alexandr Celakovsky (xcelak00@stud.fit.vutbr.cz)
 * @brief Deklarace funkci lexikalniho analyzatoru
 */

#ifndef SCANNER_H
#define SCANNER_H

typedef enum scanState {
	START,
	ID_OR_KEYWORD,
	INTEGER,
	EXPONENT,
	EXPONENT_SIGN,
	DECIMAL,
	NUMBER,
	STRING_START,
	STRING_CHECK_ASCII,
	STRING_VALID,
	STRING_BACKSLASH,
	STRING_BACKSLASH_CORRECT,
	ADD,
	SUB,
	MUL,
	DIV,
	DIV_INT,
	COMMA,
	COLON,
	PAR_L,
	PAR_R,
	LESS,
	LESS_EQ,
	GREATER,
	GREATER_EQ,
	ASSIGN,
	EQ,
	EQ_NIL_HALF,
	EQ_NIL,
	CONCAT_HALF,
	CONCAT,
	HASH

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
	T_ASSIGN,     // '='
	T_EQ,         // '=='
	T_EQ_NIL,     // '~='
	T_CONCAT,     // '..'
	T_HASH,       // '#'
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
 * @brief Zacne cist soubor
 */
int read_file();

/**
 * @brief Kontrola zda se jedna o klicove slovo
 *
 * @param token Aktualni token
 *
 */
int is_keyword(token_t *token);

/**
 * @brief Vraci 2 pokud je (a-z,A-Z), vraci 1 pokud je cislo, jinak 0
 *
 * @param char Aktualni znak 
 *
 */
int get_char_type(char c);

/**
 * @brief Nacte dalsi token
 *
 * @param token Dalsi token pro zpracovani
 *
 */
int get_token(token_t *token);

#endif
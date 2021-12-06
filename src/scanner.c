/**
 * @file scanner.c
 * @author Alexandr Celakovsky (xcelak00@stud.fit.vutbr.cz)
 * @brief Implementace lexikalniho analyzatoru
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scanner.h"

char buffer[20];

int lineCount = 1;

void token_print(token_t *token){
	printf("\nSTART Token\n");
	printf("line number: %i\n", token->line);
	printf("type: %i (z scanner.h)\n", token->type);
	if(token->type == T_KW)
		printf("keyword: %i\n", token->keyword);
	if(token->type == T_KW || token->type == T_ID || token->type == T_STRING)
		printf("data: %s\n", token->data);
	if(token->type == T_INTEGER)
		printf("integer: %lli\n", token->integer);
	if(token->type == T_NUMBER)
		printf("number: %f\n", token->number);
	printf("END Token\n");
}

int token_data_init(token_t *token){
    token->data = malloc(1);
    if(token->data == NULL){
	exit(99);
    }
    token->data[0] = '\0';

    return 0;
}

int token_data_append(token_t *token, char c){
    int len = strlen(token->data);
    token->data = realloc(token->data , len + 1 + 1);
    if(token->data == NULL){
	exit(99);
    }
    token->data[len] = c;
    token->data[len+1] = '\0';

    return 0;
}

int token_data_remove_last_char(token_t *token){
	int len = strlen(token->data);
	// Nelze mazat do minusu
	if(len != 0){
		token->data = realloc(token->data , len);
		if(token->data == NULL){
			exit(99);
		}
		token->data[len-1] = '\0';
	}
	return 0;
}

int token_data_clear(token_t *token){
    free(token->data);
    return 0;
}

int is_keyword(token_t *token){
	if(strcmp("do", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_DO;
		return 1;
	}
	else if(strcmp("else", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_ELSE;
		return 1;
	}
	else if(strcmp("end", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_END;
		return 1;
	}
	else if(strcmp("function", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_FUNCTION;
		return 1;
	}
	else if(strcmp("global", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_GLOBAL;
		return 1;
	}
	else if(strcmp("if", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_IF;
		return 1;
	}
	else if(strcmp("integer", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_INTEGER;
		return 1;
	}
	else if(strcmp("local", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_LOCAL;
		return 1;
	}
	else if(strcmp("nil", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_NIL;
		return 1;
	}
	else if(strcmp("number", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_NUMBER;
		return 1;
	}
	else if(strcmp("require", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_REQUIRE;
		return 1;
	}
	else if(strcmp("return", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_RETURN;
		return 1;
	}
	else if(strcmp("string", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_STRING;
		return 1;
	}
	else if(strcmp("then", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_THEN;
		return 1;
	}
	else if(strcmp("while", token->data) == 0){
		token->type = T_KW;
		token->keyword = KW_WHILE;
		return 1;
	}
	token->type = T_ID;
	return 0;
}

int get_char_type(char c){
	// 0-9
	if( c >= '0' && c <= '9' ){
		return 1;
	}

	// a-z,A-Z
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
		return 2;
	}

	return 0;
}

int get_token(token_t *token){
	ScanState state = START;
	char curr_char;

	while(1) {
		curr_char = getc(stdin);
		token->line = lineCount;

		switch(state){
			case START:
				switch(curr_char){
					case EOF:
						token->type = T_EOF;
						return 0;
					case '\n':
						token->type = T_EOL;
						lineCount++;
						return 0;	
					case '"':
						state = STRING_START;
						break;
					case '#':
						state = HASH;
						ungetc(curr_char, stdin);
						break;
					case '.':
						state = CONCAT_HALF;
						// Zde nebudu vracet znak do stdin, protoze hledam stejny
						// ungetc(curr_char, stdin);
						break;
					case '~':
						state = EQ_NIL_HALF;
						// ungetc(curr_char, stdin);
						break;
					case '=':
						state = ASSIGN;
						// Zde nebudu vracet znak do stdin, protoze hledam stejny
						// ungetc(curr_char, stdin);
						break;
					case '>':
						state = GREATER;
						// ungetc(curr_char, stdin);
						break;
					case '<':
						state = LESS;
						// ungetc(curr_char, stdin);
						break;
					case ')':
						state = PAR_R;
						ungetc(curr_char, stdin);
						break;
					case '(':
						state = PAR_L;
						ungetc(curr_char, stdin);
						break;
					case ':':
						state = COLON;
						ungetc(curr_char, stdin);
						break;
					case ',':
						state = COMMA;
						ungetc(curr_char, stdin);
						break;
					case '/':
						state = DIV;
						// ungetc(curr_char, stdin);
						break;
					case '*':
						state = MUL;
						ungetc(curr_char, stdin);
						break;
					case '-':
						state = SUB;
						// ungetc(curr_char, stdin);
						break;
					case '+':
						state = ADD;
						ungetc(curr_char, stdin);
						break;
					default:
						if(get_char_type(curr_char) == 2){
							state = ID_OR_KEYWORD;
							token_data_init(token);
							ungetc(curr_char, stdin);
							break;
						}
						if(get_char_type(curr_char) == 1){
							state = INTEGER;
							token_data_init(token);
							ungetc(curr_char, stdin);
							break;
						}
						else {
							// Nic jineho nelze - neni pravda lze whitespace
							// Error
							// return 1;
						}
						break;
				}
				break;

			case ID_OR_KEYWORD:
				if(get_char_type(curr_char) == 0 && curr_char != '_'){
					is_keyword(token);
					ungetc(curr_char, stdin);
					return 0;
				}
				token_data_append(token,curr_char);
				is_keyword(token);
				break;

			case INTEGER:
				token->type = T_INTEGER;
				if(curr_char == 'E'){
					state = EXPONENT;
					token_data_append(token, curr_char);
					// ungetc(curr_char, stdin);
					break;
				}

				else if(curr_char == '.'){
					state = DECIMAL;
					token_data_append(token,'.');
					// ungetc(curr_char, stdin);
					break;
				}
				// Je to cislo
				else if(get_char_type(curr_char) == 1){
					token_data_append(token, curr_char);
					char *ptr;
					long long result = strtoll(token->data, &ptr,10);
					token->integer = result;
					break;
				}
				// Jiny znak, vratim ho do stdin
				else {

					ungetc(curr_char, stdin);
					return 0;
				}
				break;

			case EXPONENT:
				// Zde ocekavam pouze + nebo -
				if(curr_char == '+' || curr_char == '-'){
					state = EXPONENT_SIGN;
					token_data_append(token,curr_char);
					// ungetc(curr_char, stdin);
					break;
				}
				// Muze byt jeste cislo
				else if(get_char_type(curr_char) == 1){
					state = EXPONENT_SIGN;
					ungetc(curr_char, stdin);
					break;
				}
				// Zadne jine znaky mit nemuzu
				else {
					// Chyba
					return 1;
				}
				break;

			case EXPONENT_SIGN:
				// Zde ocekavam pouze cislo
				if(get_char_type(curr_char) == 1){
					state = NUMBER;
					ungetc(curr_char, stdin);
					break;
				}
				// Zadne jine znaky mit nemuzu
				else {
					// Chyba
					return 1;
				}
				break;
			
			case DECIMAL:
				// Zde ocekavam pouze cislo
				if(get_char_type(curr_char) == 1){
					state = NUMBER;
					ungetc(curr_char, stdin);
					break;
				}
				// Zadne jine znaky mit nemuzu
				else {
					// Chyba
					return 1;
				}
				break;
				

			case NUMBER:
				// Spravny znak, opakuju
				if(get_char_type(curr_char) == 1){
					token->type = T_NUMBER;
					token_data_append(token, curr_char);
					double number = strtod(token->data,NULL);
					token->number = number;
					break;
				}
				else {
					token_data_clear(token);
					return 0;
				}
				break;
			
			case HASH:
				token->type = T_HASH;
				// Vracim token
				return 0;

			case CONCAT_HALF:
				if(curr_char == '.'){
					state = CONCAT;
					ungetc(curr_char, stdin);
					break;
				}
				// Jiny znak neocekavam
				// napr .+
				else {
					return 1;
				}
				break;
			
			case CONCAT:
				token->type = T_CONCAT;
				// Vracim token
				return 0;
			
			case EQ_NIL_HALF:
				// Ocekavam pouze =
				if(curr_char == '='){
					state = EQ_NIL;
					ungetc(curr_char, stdin);
					break;
				}
				// Jiny znak neocekavam
				else {
					// Error
					return 1;
				}
				break;
			
			case EQ_NIL:
				token->type = T_EQ_NIL;
				// Vracim token
				return 0;

			case ASSIGN:
				// Bude to '=='
				if(curr_char == '='){
					state = EQ;
					ungetc(curr_char, stdin);
					break;
				}
				// bude to '=' a znak vracim na vstup
				else {
					ungetc(curr_char, stdin);
					token->type = T_ASSIGN;
					// Vracim token
					return 0;
				}
				break;

			case EQ:
				token->type = T_EQ;
				// Vracim token
				return 0;
			
			case GREATER:
				if(curr_char == '=') {
					state = GREATER_EQ;
					ungetc(curr_char, stdin);
					break;
				}
				else {
					ungetc(curr_char, stdin);
					token->type = T_GREATER;
					// Vracim token
					return 0;
				}
			
			case GREATER_EQ:
				token->type = T_GREATER_EQ;
				// Vracim token
				return 0;

			case LESS:
				if(curr_char == '=') {
					state = LESS_EQ;
					ungetc(curr_char, stdin);
					break;
				}
				else {
					ungetc(curr_char, stdin);
					token->type = T_LESS;
					// Vracim token
					return 0;
				}
			
			case LESS_EQ:
				token->type = T_LESS_EQ;
				// Vracim token
				return 0;

			case PAR_R:
				token->type = T_PAR_R;
				// Vracim token
				return 0;
				
			case PAR_L:
				token->type = T_PAR_L;
				// Vracim token
				return 0;

			case COLON:
				token->type = T_COLON;
				// Vracim token
				return 0;

			case COMMA:
				token->type = T_COMMA;
				// Vracim token
				return 0;

			case DIV:
				// Bude to '//'
				if(curr_char == '/'){
					state = DIV_INT;
					ungetc(curr_char, stdin);
					break;
				}
				// bude to '/' a znak vracim na vstup
				else {
					ungetc(curr_char, stdin);
					token->type = T_DIV;
					// Vracim token
					return 0;
				}
				break;

			case DIV_INT:
				token->type = T_DIV_INT;
				// Vracim token
				return 0;
			
			case MUL:
				token->type = T_MUL;
				// Vracim token
				return 0;

			case SUB:
				// Je to jenom '-'
				if(curr_char != '-'){
					ungetc(curr_char, stdin);
					token->type = T_SUB;
					// Vracim token
					return 0;
				}
				// Je to komentar
				else {
					state = COMMENT_DECIDE;
					break;
				}
			
			case ADD:
				token->type = T_ADD;
				// Vracim token
				return 0;
			
			case STRING_START:
				// Neukonceny string = chyba
				if(curr_char == '\n' || curr_char == EOF){
					return 1;
				}
				if(curr_char == '"'){
					// Vstup "" -> initializuji prazdny string
					token_data_init(token);
					token->type = T_STRING;
					return 0;
				}
				else {
					state = STRING_CHECK_ASCII;
					token_data_init(token);
					ungetc(curr_char, stdin);
					break;
				}
			
			case STRING_CHECK_ASCII:
				// Neukonceny string = chyba
				if(curr_char == '\n' || curr_char == EOF){
					return 1;
				}
				if((curr_char >= ' ') && (curr_char != '"')){
					token_data_append(token, curr_char);
					state = STRING_VALID;
					break;
				}
				else {
					// Error
					return 1;
				}
				break;
			
			case STRING_VALID:
				// Neukonceny string == chyba
				if(curr_char == '\n' || curr_char == EOF){
					return 1;
				}
				if(curr_char == '\\'){
					state = STRING_BACKSLASH;
					token_data_append(token, curr_char);
					break;
				}
				else if(curr_char == '"'){
					token->type = T_STRING;
					// Vracim token
					return 0;
				}
				else {
					state = STRING_CHECK_ASCII;
					ungetc(curr_char, stdin);
					break;
				}

			case STRING_BACKSLASH:
				// Neukonceny string == chyba
				if(curr_char == '\n' || curr_char == EOF){
					return 1;
				}
				if(curr_char == '"' || curr_char == '\\' || curr_char == 'n' || curr_char == 't'){
					state = STRING_BACKSLASH_CORRECT;
					ungetc(curr_char, stdin);
					break;
				}
				else {
					// Error
					return 1;
				}
			
			case STRING_BACKSLASH_CORRECT:
				token_data_append(token, curr_char);
				state = STRING_VALID;
				break;
			
			case COMMENT_DECIDE:
				if(curr_char == '['){
					curr_char = getc(stdin);
					if(curr_char == '['){
						state = COMMENT_BLOCK;
						break;
					} else {
						// Chyba
						// --[*
						// nespravny komentar
						return 1;	
						break;
					}
				}
				// Radkovy komentar
				else {
					ungetc(curr_char, stdin);
					state = COMMENT_LINE;
					break;
				}

			case COMMENT_LINE:
				while(curr_char != '\n' && curr_char != EOF){
					curr_char = getc(stdin);
				}
				if(curr_char == EOF){
					token->type = T_EOF;
					return 0;
				}
				ungetc(curr_char, stdin);
				state = START;
				break;

			case COMMENT_BLOCK:
				while(1){
					if(curr_char == ']'){
						curr_char = getc(stdin);
						if(curr_char == EOF){
							token->type = T_EOF;
							return 0;
						}
						if(curr_char == ']'){
							state = START;
							break;
						}
					}
					if(curr_char == EOF){
						token->type = T_EOF;
						return 0;
					}
					curr_char = getc(stdin);
				}
				break;
		}

		if(curr_char == EOF){
			return 0;
		}
	}

	return 0;
}

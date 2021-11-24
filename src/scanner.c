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

void token_print(token_t *token){
	printf("\nSTART Token\n");
	printf("type: %i\n", token->type);
	if(token->type == T_KW)
		printf("keyword: %i\n", token->keyword);
	if(token->type == T_KW || token->type == T_ID)
		printf("data: %s\n", token->data);
	if(token->type == T_INTEGER)
		printf("integer: %i\n", token->integer);
	if(token->type == T_NUMBER)
		printf("number: %f\n", token->number);
	printf("END Token\n");
}

int token_data_init(token_t *token){
    token->data = malloc(1);
    if(token->data == NULL){
        // TODO chyba
        return -1;
    }
    token->data[0] = '\0';

    return 0;
}

int token_data_append(token_t *token, char c){
    int len = strlen(token->data);
//     printf("STRLEN: %i\n", len);
    token->data = realloc(token->data , len + 1 + 1);
    // TODO check realloc success
    token->data[len] = c;
    token->data[len+1] = '\0';

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
		token->keyword = KW_NIL;
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

		switch(state){
			case START:
				switch(curr_char){
					case '"':
						state = STRING_START;
						ungetc(curr_char, stdin);
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
						ungetc(curr_char, stdin);
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
						break;
				}
				break;

			case ID_OR_KEYWORD:
				if((get_char_type(curr_char) == 0) && (curr_char != '_')){
					is_keyword(token);
					return 0;
				}
				token_data_append(token,curr_char);
				break;

			case INTEGER:
				token->type = T_INTEGER;
				if(curr_char == 'E'){
					state = EXPONENT;
					ungetc(curr_char, stdin);
					break;
				}

				if(curr_char == ','){
					state = DECIMAL;
					ungetc(curr_char, stdin);
					break;
				}

				// Je to cislo
				if(get_char_type(curr_char) == 1){
					token_data_append(token, curr_char);
					char *ptr;
					long result = strtol(token->data, &ptr,10);
					token->integer = (int) result;
					break;
				}
				// Ocekavam pouze mezeru a jine ukonceni?
				// TODO zkontrolovat spravnost teto podminky
				else if(curr_char == ' ' || curr_char == '\n' || curr_char == EOF ){
					// Vracim token
					return 0;
				}
				// Zadne jine znaky mit nemuzu
				else {
					// TODO Error
				}
				break;

			case EXPONENT:
				// Spravny znak, opakuju
				if(curr_char == 'E'){
					token_data_append(token, curr_char);
					break;
				}
				// Zde ocekavam pouze + nebo -
				else if(curr_char == '+' || curr_char == '-'){
					state = EXPONENT_SIGN;
					ungetc(curr_char, stdin);
					break;
				}
				// Zadne jine znaky mit nemuzu
				else {
					// TODO Error
				}
				break;

			case EXPONENT_SIGN:
				// Spravny znak, opakuju
				if(curr_char == '+' || curr_char == '-'){
					token_data_append(token, curr_char);
					break;
				}
				// Zde ocekavam pouze cislo
				else if(get_char_type(curr_char) == 1){
					state = NUMBER;
					ungetc(curr_char, stdin);
					break;
				}
				// Zadne jine znaky mit nemuzu
				else {
					// TODO Error
				}
				break;
			
			case DECIMAL:
				// Spravny znak, opakuju
				if(curr_char == ','){
					token_data_append(token, curr_char);
					break;
				}
				// Zde ocekavam pouze cislo
				else if(get_char_type(curr_char) == 1){
					state = NUMBER;
					ungetc(curr_char, stdin);
					break;
				}
				// Zadne jine znaky mit nemuzu
				else {
					// TODO Error
				}
				break;
				

			case NUMBER:
				// Spravny znak, opakuju
				if(get_char_type(curr_char) == 1){
					token->type = T_NUMBER;
					token_data_append(token, curr_char);
					break;
				}
				// Ocekavam pouze mezeru a jine ukonceni?
				// TODO zkontrolovat spravnost teto podminky
				else if(curr_char == ' ' || curr_char == '\n' || curr_char == EOF){
					// Vracim token
					return 0;
				}
				// Zadne jine znaky mit nemuzu
				else {
					// TODO Error
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
				else {
					// TODO Error
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
					// TODO Error
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
				// Vracaim token
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
				token->type = T_SUB;
				// Vracim token
				return 0;
			
			case ADD:
				token->type = T_ADD;
				// Vracim token
				return 0;
		}

		if(curr_char == EOF){
			return;
		}
	}

	return 0;
}
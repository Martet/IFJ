/**
 * @file scanner.c
 * @author Alexandr Celakovsky (xcelak00@stud.fit.vutbr.cz)
 * @brief Implementace lexikalniho analyzatoru
 */

#include <stdio.h>
#include <string.h>
#include "scanner.h"

char buffer[20];

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
    printf("STRLEN: %i\n", len);
    token->data = realloc(token->data , len + 1 + 1);
    // TODO check realloc success
    token->data[len] = c;
    token->data[len+1] = '\0';

    return 0;
}

int token_data_clear(token_t *token){
    free(token->data);
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
		curr_char = getchar();

		switch(state){
			case START:
				switch(curr_char){
					case '"':
						state = STRING_START;
						putchar(curr_char);
						break;
					case '#':
						state = HASH;
						putchar(curr_char);
						break;
					case '.':
						state = CONCAT_HALF;
						putchar(curr_char);
						break;
					case '~':
						state = EQ_NIL_HALF;
						putchar(curr_char);
						break;
					case '=':
						state = ASSIGN;
						putchar(curr_char);
						break;
					case '>':
						state = GREATER;
						putchar(curr_char);
						break;
					case '<':
						state = LESS;
						putchar(curr_char);
						break;
					case ')':
						state = PAR_R;
						putchar(curr_char);
						break;
					case '(':
						state = PAR_L;
						putchar(curr_char);
						break;
					case ':':
						state = COLON;
						putchar(curr_char);
						break;
					case ',':
						state = COMMA;
						putchar(curr_char);
						break;
					case '/':
						state = DIV;
						putchar(curr_char);
						break;
					case '*':
						state = MUL;
						putchar(curr_char);
						break;
					case '-':
						state = SUB;
						putchar(curr_char);
						break;
					case '+':
						state = ADD;
						putchar(curr_char);
						break;
					default:
						if(get_char_type(curr_char) == 2){
							state = ID_OR_KEYWORD;
							putchar(curr_char);
							break;
						}
						if(get_char_type(curr_char) == 1){
							state = INTEGER;
							putchar(curr_char);
							break;
						}
						break;
				}
				break;

			case ID_OR_KEYWORD:
				if((get_char_type(curr_char) == 0) && (curr_char != '_')){
					printf(" = ID or KW\n");
					state = START;
					break;
				}
				printf("%c",curr_char);
				break;
		}



		if(curr_char == EOF){
			return;
		}
	}

	return 0;
}
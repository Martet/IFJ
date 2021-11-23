/**
 * @file scanner.c
 * @author Alexandr Celakovsky (xcelak00@stud.fit.vutbr.cz)
 * @brief Implementace lexikalniho analyzatoru
 */

#include <stdio.h>
#include "scanner.h"

char buffer[20];

FILE *file;

int is_keyword(token_t *token){
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
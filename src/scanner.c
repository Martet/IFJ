/**
 * @file scanner.c
 * @author Alexandr Celakovsky (xcelak00@stud.fit.vutbr.cz)
 * @brief Implementace lexikalniho analyzatoru
 */

#include <stdio.h>
#include "scanner.h"

char buffer[20];

FILE *file;

int read_file(){
	file = fopen("test.txt","r");

	if(file == NULL){
		return -1;
	}

	return 0;
}

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

	while(1) {
		char curr_char = fgetc(file);

		switch(state){
			case START:
				switch(curr_char){
					case '"':
						state = STRING_START;
						ungetc(curr_char, file);
						break;
					case '#':
						state = HASH;
						ungetc(curr_char, file);
						break;
					case '.':
						state = CONCAT_HALF;
						ungetc(curr_char, file);
						break;
					case '~':
						state = EQ_NIL_HALF;
						ungetc(curr_char, file);
						break;
					case '=':
						state = ASSIGN;
						ungetc(curr_char,file);
						break;
					case '>':
						state = GREATER;
						ungetc(curr_char,file);
						break;
					case '<':
						state = LESS;
						ungetc(curr_char,file);
						break;
					case ')':
						state = PAR_R;
						ungetc(curr_char,file);
						break;
					case '(':
						state = PAR_L;
						ungetc(curr_char,file);
						break;
					case ':':
						state = COLON;
						ungetc(curr_char,file);
						break;
					case ',':
						state = COMMA;
						ungetc(curr_char,file);
						break;
					case '/':
						state = DIV;
						ungetc(curr_char,file);
						break;
					case '*':
						state = MUL;
						ungetc(curr_char,file);
						break;
					case '-':
						state = SUB;
						ungetc(curr_char,file);
						break;
					case '+':
						state = ADD;
						ungetc(curr_char,file);
						break;
					default:
						if(get_char_type(curr_char) == 2){
							state = ID_OR_KEYWORD;
							ungetc(curr_char,file);
							break;
						}
						if(get_char_type(curr_char) == 1){
							state = INTEGER;
							ungetc(curr_char,file);
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
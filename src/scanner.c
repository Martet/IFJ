/**
 * @file scanner.c
 * @author Alexandr Celakovsky (xcelak00@stud.fit.vutbr.cz)
 * @brief Implementace lexikalniho analyzatoru
 */

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

	switch(state){

	}

	return 0;
}
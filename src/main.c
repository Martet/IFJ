#include <stdio.h>
#include "parser.h"
#include "scanner.h"

int main(void){
    token_t token;
    read_file();
    get_token(&token);
    return prog(&token);
}

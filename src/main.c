#include <stdio.h>
#include "parser.h"
#include "scanner.h"

int main(void){
    token_t token;
    get_token(&token);
    return prog(&token);
}

#include <stdio.h>
#include "parser.h"
#include "scanner.h"
#include "expression.h"

int main(void){
    token_t token;
    // get_token(&token);
    // Priklad vraceni vsech tokenu
    // while (token.type != T_EOF)
    // {
    // get_token(&token);  
    // token_print(&token);
    //   }
    
    get_token(&token);
    solvedExpression(&token);
    return 0;//prog(&token);
}

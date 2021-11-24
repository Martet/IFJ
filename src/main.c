#include <stdio.h>
#include "parser.h"
#include "scanner.h"

int main(void){
    token_t token;

    // Priklad vraceni vsech tokenu
    while(token.type != T_EOF){
        get_token(&token);
        token_print(&token);
        if(token.type == T_KW || token.type == T_ID){
            token_data_clear(&token);
        }
    }

    return prog(&token);
}

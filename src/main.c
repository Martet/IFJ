#include <stdio.h>
#include "parser.h"
#include "scanner.h"
#include "expression.h"

ErrCode printError(ErrCode err, token_t *token){
    switch(err){
        case ERR_LEX:
            fprintf(stderr, "Radek %d: Chyba pri lexikalni analyze\n", token->line);
            return err;
        case ERR_PARSE:
            fprintf(stderr, "Radek %d: Chyba pri syntakticke analyze\n", token->line);
            return err;
        case ERR_SEM_DEF:
            fprintf(stderr, "Radek %d: Chyba pri semanticke analyze - definice\n", token->line);
            return err;
        case ERR_SEM_ASSIGN:
            fprintf(stderr, "Radek %d: Chyba pri semanticke analyze - prirazeni\n", token->line);
            return err;
        case ERR_SEM_PARAM:
            fprintf(stderr, "Radek %d: Chyba pri semanticke analyze - parametry funkce\n", token->line);
            return err;
        case ERR_SEM_TYPE:
            fprintf(stderr, "Radek %d: Chyba pri semanticke analyze - vyraz\n", token->line);
            return err;
        case ERR_NIL:
            fprintf(stderr, "Radek %d: Chyba pri semanticke analyze - neocekavany nil\n", token->line);
            return err;
        case ERR_ZERO:
            fprintf(stderr, "Radek %d: Chyba pri semanticke analyze - deleni nulou\n", token->line);
            return err;
        default:
            return err;
    }
}

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
    return printError(solvedExpression(&token), &token);
}

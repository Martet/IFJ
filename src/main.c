/**
 * @file main.c
 * @author Dominik Klon (xklond00@stud.fit.vutbr.cz)
 * @author Martin Zmitko (xzmitk01@stud.fit.vutbr.cz)
 * @author Petr Hýbl (xhyblp01@stud.fit.vutbr.cz)
 * @author Alexandr Čelakovský (xcelak00@stud.fit.vutbr.cz)
 * @brief Vstupni bod do prekladace
 */

#include <stdio.h>
#include "parser.h"
#include "scanner.h"
#include "expression.h"
#include "symtable.h"

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

void setup_func(char *name, char *params, char *types){
    tableItem_t *item = table_insert(&global_table, name);
    item->defined = true;
    item->params = params;
    item->types = types;
}

int main(void){
    printf(".IFJcode21\n");
    printf("DEFVAR GF@op1\n");
    printf("DEFVAR GF@op2\n");
    printf("DEFVAR GF@op3\n");
    printf("JUMP $CALLS$\n");

    setup_func("write", "w", "");
    setup_func("reads", "", "S");
    setup_func("readi", "", "I");
    setup_func("readn", "", "N");
    setup_func("substr", "SNN", "S");
    setup_func("ord", "SI", "I");
    setup_func("chr", "I", "S");

    token_t token;
    NEXT_TOKEN(&token);
    return printError(prog(&token), &token);
}

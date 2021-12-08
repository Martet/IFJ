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

void print_builtin(){
    printf(
        "LABEL write\n"
        "POPS GF@op2\n"
        "LABEL write$while\n"
        "POPS GF@op1\n"
        "WRITE GF@op1\n"
        "SUB GF@op2 GF@op2 int@1\n"
        "JUMPIFNEQ write$while GF@op2 int@0\n"
        "RETURN\n"
        "\n"
        "LABEL tointeger\n"
        "POPS GF@op1\n"
        "FLOAT2INT GF@op2 GF@op1\n"
        "PUSHS GF@op2\n"
        "RETURN\n"
        "\n"
        "LABEL reads\n"
        "READ GF@op1 string\n"
        "PUSHS GF@op1\n"
        "RETURN\n"
        "\n"
        "LABEL readi\n"
        "READ GF@op1 int\n"
        "PUSHS GF@op1\n"
        "RETURN\n"
        "\n"
        "LABEL readn\n"
        "READ GF@op1 float\n"
        "PUSHS GF@op1\n"
        "RETURN\n"
        "\n"
        "LABEL substr\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@s\n"
        "DEFVAR LF@i\n"
        "DEFVAR LF@j\n"
        "DEFVAR LF@compare\n"
        "DEFVAR LF@length\n"
        "DEFVAR LF@index\n"
        "DEFVAR LF@output\n"
        "MOVE LF@output string@\n"
        "DEFVAR LF@tempchar\n"
        "POPS LF@s\n"
        "POPS LF@i\n"
        "POPS LF@j\n"
        "JUMPIFEQ isnil LF@s nil@nil\n"
        "JUMPIFEQ isnil LF@i nil@nil\n"
        "JUMPIFEQ isnil LF@j nil@nil\n"
        "STRLEN LF@length LF@s\n"
        "GT LF@compare LF@i LF@length\n"
        "JUMPIFEQ returnnil LF@compare bool@true\n"
        "GT LF@compare LF@j LF@length\n"
        "JUMPIFEQ returnnil LF@compare bool@true\n"
        "LT LF@compare LF@i int@1\n"
        "JUMPIFEQ returnnil LF@compare bool@true\n"
        "LT LF@compare LF@j int@1\n"
        "JUMPIFEQ returnnil LF@compare bool@true\n"
        "LT LF@compare LF@j LF@i\n"
        "JUMPIFEQ returnnil LF@compare bool@true\n"
        "MOVE LF@index LF@i\n"
        "SUB LF@index LF@index int@1\n"
        "LABEL while\n"
        "GETCHAR LF@tempchar LF@s LF@index\n"
        "CONCAT LF@output LF@output LF@tempchar\n"
        "ADD LF@index LF@index int@1\n"
        "JUMPIFNEQ while LF@index LF@j\n"
        "PUSHS LF@output\n"
        "POPFRAME\n"
        "RETURN\n"
        "LABEL returnnil\n"
        "PUSHS nil@nil\n"
        "POPFRAME\n"
        "RETURN\n"
        "LABEL isnil\n"
        "EXIT int@8\n"
        "POPFRAME\n"
        "RETURN\n\n");
}

int main(void){
    printf(".IFJcode21\n");
    printf("DEFVAR GF@op1\n");
    printf("DEFVAR GF@op2\n");
    printf("DEFVAR GF@op3\n");
    printf("JUMP $CALLS$\n\n");

    print_builtin();
    setup_func("write", "w", "");
    setup_func("tointeger", "N", "I");
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

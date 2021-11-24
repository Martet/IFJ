/**
 * @file expression.c
 * @brief Syntakticka analyza pro výrazy
 * @author Petr Hýbl <xhyblp01@stud.fit.vutbr.cz>
 */
#include <stdio.h>
#include <stdbool.h>
#include "stack.h"
#include "scanner.h"
#include "parser.h"
#include "expression.h"
#define TABLE_SIZE 6

const char Precedence_table[TABLE_SIZE][TABLE_SIZE] = {
//  |+-|*///| ( | ) | i | $ 
    {'>','<','<','<','<','>'}, // +-
    {'>','>','<','<','<','>'}, // *///
    {'<','<','<','=','<','0'}, // (
    {'>','>','0','>','0','>'}, // )
    {'>','>','0','>','0','>'}, // i
    {'<','<','<','0','<','D'}, // $ 
};


int get_index_to_table(token_t *token)
{
    int index = -1;
    switch (token->type)
    {
    case T_ID:
        index = 4;
        break;
    case T_ADD:
        index = 0;
        break;
    case T_SUB:
        index = 0;
        break;
    case T_MUL:
        index = 1;
        break;
    case T_DIV:
        index = 1;
        break;
    case T_DIV_INT:
        index = 1;
    case T_PAR_L:
        index = 2;
    case T_PAR_R:
        index = 3;
    //case 

    default:
        break;
    }


}

int solvedExpression(token_t *token){
 Stack stack;
 Stack_Init(&stack);
 token_t token;
 bool end = false;
 Stack_Push(&stack,'D');
 int lastindex = 5;
 int secondindex = -1;
 while (end == false)
 {
    NEXT_TOKEN(token);
    secondindex = get_index_to_table(&token);

    
 }
 

 
 
 
 


}
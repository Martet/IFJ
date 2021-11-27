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
#include <stdlib.h>
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



int stack_to_table(Stack *s)
{
    TokenType A = Stack_Top(s);
    switch (A)
    {
    case T_EOL:
        printf("dolaaar");
        return 5;
        break;
    case T_ADD:
    
    default:
        break;
    }

    return 0;

}
int get_index_to_table(TokenType type)
{
    int index = -1;
    char B = 0;  // enum type
    printf("pico jsem tady");
    switch (type)
    {
    case T_ID:
        index = 4;
        break;
    case T_ADD:
        index = 0;
        B = '+';
        break;
    case T_SUB:
        index = 0;
        B = '-';
        break;
    case T_MUL:
        index = 1;
        B = '*';
        break;
    case T_DIV:
        index = 1;
        B = '/';
        break;
    case T_DIV_INT:
        B = 'Z';
        index = 1;
        break;
    case T_PAR_L:
        index = 2;
        B = '(';
        break;
    case T_PAR_R:
        index = 3;
        B = ')';
        break;
    case T_EOL:
        index = 5;
        B='D';
        break;
    default:
        break;
    }

    
    return index;

}
IdentType TokentoIden(TokenType type){
    switch (type)
    {
    case T_ID:
        return I_ID;
        break;
    case T_ADD:
        return I_PLUS;
        break;
    case T_SUB:
        return I_MINUS;
        break;
    case T_MUL:
        return I_MULTIPLAY;
        break;
    case T_DIV:
        return I_DIVIDE;
        break;
    case T_DIV_INT:
        return I_DIVIDE_INT;
        break;
    case T_PAR_L:
        return I_PAR_L;
        break;
    case T_PAR_R:
        return I_PAR_R;
        break;
    case T_EOL:
        return I_DOLAR;
        break;
    
    default:
        break;
    }
    return I_EROR;

}


void reduce(Stack* stack)
{
    ptrItem* topik = stack->top;
    int cnt = 0;
    while (topik->data != I_HALT)
    {
        topik = topik->next;
        cnt++;

    }
    if (cnt==1 && stack->top->data==I_ID)
    {
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM);
        printf("E \n");
    }
    if (cnt==3 && stack->top->data==I_NON_TERM && stack->top->next->data == I_PLUS && stack->top->next->next->data == I_NON_TERM)
    {
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM);
        printf("E + E --> E \n");
        //generace operace stack->top->next->data
    }

    
    
    


}

int solvedExpression(token_t *token){

 Stack s;

 Stack_Init(&s);

 Stack_Push(&s,I_DOLAR);

 IdentType LastIDENTS = I_DOLAR;
 
 while (token->type != T_EOL)
 {
     int a = stack_to_table(&s);
     int b = get_index_to_table(token->type);
     IdentType B = TokentoIden(token->type);
      printf("   indexy  %d , %d \n",a,b);
    switch (Precedence_table[a][b])
    {
    case '=':
         Stack_Push(&s,B);
         printf("jsem tu = ");
         LastIDENTS = B;
         NEXT_TOKEN(token);
        break;
    case '<':
        printf("jsem tu <");
        Stack_Push(&s,I_HALT);
        Stack_Push(&s,B);
        LastIDENTS = B;
        NEXT_TOKEN(token);
        break;
    case '>':
         printf("jsem tu >");
         reduce(&s);
         
        break;
    default:
        return -1;
        break;
    }
 }
 


    Stack_Destroy(&s);
 return 0;
 
}
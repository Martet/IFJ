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
    {'>','<','<','>','<','>'}, // +-
    {'>','>','<','>','<','>'}, // *///
    {'<','<','<','=','<','0'}, // (
    {'>','>','0','>','0','>'}, // )
    {'>','>','0','>','0','>'}, // i
    {'<','<','<','0','<','D'}, // $ 
};

 Stack s;

int stack_to_table(Stack *s)
{
    IdentType A = Stack_Top(s);
    if (A == I_NON_TERM || A == I_HALT)
    {
        ptrItem* topicek = s->top;
        while (topicek->data == I_NON_TERM || topicek->data == I_HALT )
        {
            topicek = topicek->next;
        }
        A= topicek->data;
    }
    
    int index = -1;
    switch (A)
    {
    case I_ID:
        index = 4;
        break;
    case I_NUMBER:
        index = 4;
        break;
    case I_STRING:
        index = 4;
        break;
    case I_INTEGER:
        index = 4;
        break;                
    case I_PLUS:
        index = 0;
        break;
    case I_MINUS:
        index = 0;
        break;
    case I_MULTIPLAY:
        index = 1;
        break;
    case I_DIVIDE:
        index = 1;
        break;
    case I_DIVIDE_INT:
        index = 1;
        break;
    case I_PAR_L:
        index = 2;
        break;
    case I_PAR_R:
        index = 3;
        break;
    case I_DOLAR:
        index = 5;
        break;


         
    default:

        break;
    }

    return index;

}
int get_index_to_table(TokenType type)
{
    int index = -1;
    char B = 0;  // enum type
    switch (type)
    {
    case T_ID:
        index = 4;
        break;
    case T_NUMBER:
        index = 4;
        break;
    case T_INTEGER:
        index = 4;
        break;
    case T_STRING:
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
    case T_NUMBER:
        return I_NUMBER;
    case T_STRING:
        return I_STRING;
    case T_INTEGER:
        return I_INTEGER;
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
    if (topik->data == I_PAR_R && topik->next->next->data == I_PAR_L && topik->next->data == I_NON_TERM)
    {
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM);
        printf("(E) --> E \n");
        return;
    }
    
    while (topik->data != I_HALT)
    {
        topik = topik->next;
        cnt++;
        

        


    }
    if (cnt==1 && ( stack->top->data==I_ID || stack->top->data==I_NUMBER  || stack->top->data==I_STRING || stack->top->data==I_INTEGER))
    {
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM);
        printf("E \n");
        //generace E
    }
    if (cnt==3 && stack->top->data==I_NON_TERM && stack->top->next->data == I_PLUS && stack->top->next->next->data == I_NON_TERM)
    {
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM);
        printf("E + E --> E \n");
        //generace operace stack->top->next->data
    }
    if (cnt==3 && stack->top->data==I_NON_TERM && stack->top->next->data == I_MULTIPLAY && stack->top->next->next->data == I_NON_TERM)
    {
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM);
        printf("E * E --> E \n");
        //generace operace stack->top->next->data
    }
    if (cnt==3 && stack->top->data==I_NON_TERM && stack->top->next->data == I_MINUS && stack->top->next->next->data == I_NON_TERM)
    {
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM);
        printf("E - E --> E \n");
        //generace operace stack->top->next->data
    }

    
    
    


}

int solvedExpression(token_t *token){


    Stack_Init(&s);
    Stack_Push(&s,I_DOLAR);
    
    
 IdentType LastIDENTS = I_DOLAR;
 bool end = false;
 while (!end)
 {
     int a = stack_to_table(&s);
     int b;
     IdentType B;
    if (token->type == T_EOL || token->type == T_EOF)
    {
        b = 5;
        B = I_DOLAR;
        end= true;

        
    }
    else{
        b = get_index_to_table(token->type);
        B = TokentoIden(token->type);
    }

      printf("  \n indexy  %d , %d \n",a,b);
    switch (Precedence_table[a][b])
    {
    case '=':
         Stack_Push(&s,B);
         printf("jsem tu = \n");
         LastIDENTS = B;
         get_token(token);
        break;
    case '<':
        printf("jsem tu <  ");
        Stack_InsertBeforeNonTerm(&s,I_HALT);
        Stack_Push(&s,B);
        Stack_Print(&s); //tisknu
        LastIDENTS = B;
        token_print(token);
        get_token(token);
       token_print(token);
        break;
    case '>':
         printf("jsem tu >  reduction \n ");

         
         reduce(&s);
         Stack_Print(&s);
        break;
    case 'D':
        printf("redukujuu");
        reduce(&s);
        break;
    default:
        break;
    }
    
 }
 printf("konecna vystupovat \n");
 Stack_Print(&s);

int a = stack_to_table(&s);
while (a != 5)
{
    reduce(&s);
    a = stack_to_table(&s);
    Stack_Print(&s);
}



  Stack_Destroy(&s);
 return 0;
 
}
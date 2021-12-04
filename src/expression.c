/**
 * @file expression.c
 * @brief Syntakticka analyza pro výrazy
 * @author Petr Hýbl <xhyblp01@stud.fit.vutbr.cz>
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stack.h"
#include "scanner.h"
#include "parser.h"
#include <stdlib.h>
#include "expression.h"

#define TABLE_SIZE 9
bool boolen;

const char Precedence_table[TABLE_SIZE][TABLE_SIZE] = {
//  |+-|*///| ( | ) | i | $ |<>  |.. |#|
    {'>','<','<','>','<','>','>','>','<'}, // +-
    {'>','>','<','>','<','>','>','>','<'}, // *///
    {'<','<','<','=','<','0','<','>','<'}, // (
    {'>','>','0','>','0','>','>','<','0'}, // )
    {'>','>','0','>','0','>','>','>','0'}, // i
    {'<','<','<','0','<','D','<','<','<'}, // $
    {'>','<','<','>','<','>','>','<','<'},  // <>
    {'<','<','<','>','<','>','>','<','<'},  // ..
    {'>','>','<','>','<','>','>','>','<'},  // #
};

 Stack s;



int stack_to_table(Stack *s)
{
    IdentType A = Stack_Top_Type(s);
    if (A == I_NON_TERM || A == I_HALT)
    {
        ptrItem* topicek = s->top;
        while (topicek->type == I_NON_TERM || topicek->type == I_HALT )
        {
            topicek = topicek->next;
        }
        A= topicek->type;
    }
    
    int index = -1;
    switch (A)
    {
    case I_ID: case I_NUMBER: case I_STRING: case I_INTEGER:
        index = 4;
        break;                
    case I_PLUS: case I_MINUS:
        index = 0;
        break;
    case I_MULTIPLAY: case I_DIVIDE:  case I_DIVIDE_INT:
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
    case I_LESS: case I_LESS_EQ: case I_GREATER: case I_GREATER_EQ: case I_EQ: case I_EQ_NIL:
        index = 6;
        break;
    case I_HASH:
        index = 8;
        break;
    case I_CONCAT:
        index = 7;
        break;
    default:
        break;
    }
    return index;

}
int get_index_to_table(TokenType type)
{
    int index = -1;
    switch (type)
    {
    case T_ID: case T_NUMBER: case T_INTEGER: case T_STRING:
        index = 4;
        break;
    case T_ADD:  case T_SUB:
        index = 0;
        break;
    case T_MUL:  case T_DIV:  case T_DIV_INT:
        index = 1;
        break;
    case T_PAR_L:
        index = 2;
        break;
    case T_PAR_R:
        index = 3;
        break;
    case T_EOL:
        index = 5;
        break;
    case T_LESS: case T_LESS_EQ: case T_GREATER: case T_GREATER_EQ: case T_EQ: case T_EQ_NIL:
        index = 6;
        boolen = true;
        break;
    case T_HASH:
        index = 8;
        break;
    case T_CONCAT:
        index = 7;
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
        break;
    case T_LESS:
        return I_LESS;
        break;
    case T_LESS_EQ:
        return I_LESS_EQ;
        break;
    case T_GREATER:
        return I_GREATER;
        break;  
    case T_GREATER_EQ:
        return I_GREATER_EQ;
        break;
    case T_EQ:
        return I_EQ;
        break;
    case T_EQ_NIL:
        return I_EQ_NIL;
        break;
    case T_CONCAT:
        return I_CONCAT;
        break;
    case T_HASH:
        return I_HASH;
        break;
    default:
        break;
    }
    return I_EROR;

}




void reduce(Stack* stack,IdentType typevar)
{
    ptrItem* topik = stack->top;
    int cnt = 0;
    if (topik->type == I_PAR_R && topik->next->next->type == I_PAR_L && topik->next->type == I_NON_TERM)
    {
        Stack_Pop(stack);
        char* data = Stack_Top_Data(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM,data);
        printf("(E) --> E \n");
        return;
    }
    
    while (topik->type != I_HALT) //<
    {
        topik = topik->next;
        cnt++;
    }
    if (cnt==1 && (stack->top->type==I_ID || stack->top->type==I_NUMBER || stack->top->type==I_STRING || stack->top->type==I_INTEGER))
    {
        char* data = stack->top->data;
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM,data);
        printf("E \n");
        //generace E
    }
    else if(cnt==2 && stack->top->type==I_NON_TERM && stack->top->next->type == I_HASH)
    {
        if (typevar != I_STRING)
        {
            // printf("what neni stringois");
            exit(6);
        }
        
        char* data = Stack_Top_Data(stack); 
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM,data);
        printf("#E --> E \n");

    }
    else if (cnt==3 && stack->top->type==I_NON_TERM && stack->top->next->type >= I_PLUS && stack->top->next->type <= I_DIVIDE_INT && stack->top->next->next->type == I_NON_TERM)
    {
        if ((stack->top->next->type == I_DIVIDE || stack->top->next->type == I_DIVIDE_INT) && stack->top->data[0] == '0')
        {
            // printf("pico delis nulou");
            exit(9);
        }
        char* data = NULL;
        printf("E %d E --> E \n", stack->top->next->type);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM,data);

        //generace operace stack->top->next->data
    }
    else if (cnt==3 && stack->top->type==I_NON_TERM && stack->top->next->type > I_DOLAR && stack->top->next->type < I_CONCAT && stack->top->next->next->type == I_NON_TERM)
    {
        char* data = NULL;
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM,data);
        printf("E relace.type E --> E \n");
        //generace operace stack->top->next->data
    }
    else if (cnt==3 && stack->top->type==I_NON_TERM && stack->top->next->type == I_CONCAT && stack->top->next->next->type == I_NON_TERM)
    {
        char* data = NULL;
        if (typevar != I_STRING)
        {
            exit(6);
        }
        
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM,data);
        printf("E teckatecka E --> E \n");
        //generace operace stack->top->next->data
    }
    else 
    {
        printf("hello chyba");
        exit(6);
    }
    


}


int solvedExpression(token_t *token)
{
    Stack_Init(&s);
    Stack_Push(&s,I_DOLAR,NULL);
    int typevar = 0;
    bool end = false;
    bool string = false;
    int cnt=0;
    boolen = false; //booleen
    char* Bdata;
    while (!end)
    {
        int a = stack_to_table(&s);
        int b;
        IdentType Btype;

        
        // printf("token type %d",token->type);
        if (token->type == T_STRING)
        {
            string = true;
            if (typevar == I_INTEGER || typevar == I_NUMBER)
            {
               exit(6);
            }
            
        }
        
        if (s.top->type != I_NON_TERM)
        {
            if (s.top->type == I_NUMBER || s.top->type == I_DIVIDE || typevar == I_NUMBER )
            {
                typevar = I_NUMBER;
            }
            else if(s.top->type == I_INTEGER)
            {
                typevar = I_INTEGER;
            }
            else if (s.top->type == I_STRING)
            {
                typevar = I_STRING;
            }
        }    
        if (token->type == T_CONCAT)
        {
            if (typevar != I_STRING)
            {
                exit(6);
            }
            
        }
        
        if (boolen == true && (token->keyword == KW_DO || token->keyword == KW_THEN))
        {
            printf("%d ",token->keyword);
            break;
        }
        if (token->type == T_EOL || token->type == T_EOF || token->type == T_COMMA)
        {
            b = 5;
            Btype = I_DOLAR;

            end= true;
        }
        else
        {

            b = get_index_to_table(token->type);
            Btype = TokentoIden(token->type);

            
            if (Btype == I_ID)
            {
                Bdata = malloc(99);
                if (Bdata == NULL)
                {
                exit(99);
                }
                strcpy(Bdata,token->data);
            }
            else if(Btype == I_INTEGER)
            {
                Bdata = malloc(99);
                if (Bdata == NULL)
                {
                exit(99);
                }              
                sprintf(Bdata, "%d", token->integer);
            }
            else if (Btype == I_STRING)
            {
                Bdata = malloc(99);
                if (Bdata == NULL)
                {
                exit(99);
                }            
                strcpy(Bdata,token->data);
            }
            else if (Btype == I_NUMBER)
            {
                Bdata = malloc(99);
                if (Bdata == NULL)
                {
                    exit(99);
                }
                sprintf(Bdata, "%f", token->number);
            }
            else
            {
                strcpy(Bdata,"                            ");
            }
            
        }
        printf("  \n indexy  %d , %d \n",a,b);
        switch (Precedence_table[a][b])
        {
            case '=':
                Stack_Push(&s,Btype,Bdata);
                // printf("jsem tu = \n");
                get_token(token);
                break;
            case '<':
                // printf("jsem tu <  ");
                Stack_InsertBeforeNonTerm(&s,I_HALT,NULL); //<
                Stack_Push(&s,Btype,Bdata);
                Stack_Print(&s); //tisknu
                // token_print(token);
                get_token(token);
                break;
            case '>':
                // printf("jsem tu >  reduction \n ");
                    // printf("typevar = %d",typevar);
                reduce(&s,typevar);
                Stack_Print(&s);
                break;
            case 'D':
                // printf("redukujuu");
                reduce(&s,typevar);
                break;
            default:
                printf("haha chyba");
                exit(6);
                break;
        }

    
    }
    // printf("konecna vystupovat \n");
    Stack_Print(&s);
    int a = stack_to_table(&s);
    // printf("a %d ", a);
    // printf("typevar = %d",typevar);
    while (a != 5)
    {
        reduce(&s,typevar);
        a = stack_to_table(&s);
        Stack_Print(&s);
    }
    // printf("konecna vystupovat po while \n");
    Stack_Destroy(&s);
    // printf("konecna vystupovat po destroy \n");
    // printf("typevar %d", typevar);
    Stack_Print(&s);
    
    return 0;
 
}
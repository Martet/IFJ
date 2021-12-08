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
#include "symtable.h"

#define TABLE_SIZE 9

const char Precedence_table[TABLE_SIZE][TABLE_SIZE] = {
//  |+-|*///| ( | ) | i | $ |<>  |.. |#|
    {'>','<','<','>','<','>','>','>','<'}, // +-
    {'>','>','<','>','<','>','>','>','<'}, // *///
    {'<','<','<','=','<','0','<','<','<'}, // (
    {'>','>','0','>','0','>','>','<','0'}, // )
    {'>','>','0','>','0','>','>','>','0'}, // i
    {'<','<','<','0','<','D','<','<','<'}, // $
    {'<','<','<','>','<','>','>','<','<'},  // <>
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
        A = topicek->type;
    }
    
    switch (A)
    {
    case I_ID: case I_NUMBER: case I_STRING: case I_INTEGER:
        return 4;              
    case I_PLUS: case I_MINUS:
        return 0;
    case I_MULTIPLAY: case I_DIVIDE:  case I_DIVIDE_INT:
        return 1;
    case I_PAR_L:
        return 2;
    case I_PAR_R:
        return 3;
    case I_DOLAR:
        return 5;
    case I_LESS: case I_LESS_EQ: case I_GREATER: case I_GREATER_EQ: case I_EQ: case I_EQ_NIL:
        return 6;
    case I_HASH:
        return 8;
    case I_CONCAT:
        return 7;
    default:
        return -1;
    }
}

int get_index_to_table(TokenType type)
{
    switch (type)
    {
    case T_ID: case T_NUMBER: case T_INTEGER: case T_STRING:
        return 4;
    case T_ADD:  case T_SUB:
        return 0;
    case T_MUL:  case T_DIV:  case T_DIV_INT:
        return 1;
    case T_PAR_L:
        return 2;
    case T_PAR_R:
        return 3;
    case T_EOL:
        return 5;
    case T_LESS: case T_LESS_EQ: case T_GREATER: case T_GREATER_EQ: case T_EQ: case T_EQ_NIL:
        return 6;
    case T_HASH:
        return 8;
    case T_CONCAT:
        return 7;
    default:
        return -1;
    }
}

IdentType TokentoIden(TokenType type){
    switch (type)
    {
    case T_ID:
        return I_ID;
    case T_NUMBER:
        return I_NUMBER;
    case T_STRING:
        return I_STRING;
    case T_INTEGER:
        return I_INTEGER;
    case T_ADD:
        return I_PLUS;
    case T_SUB:
        return I_MINUS;
    case T_MUL:
        return I_MULTIPLAY;
    case T_DIV:
        return I_DIVIDE;
    case T_DIV_INT:
        return I_DIVIDE_INT;
    case T_PAR_L:
        return I_PAR_L;
    case T_PAR_R:
        return I_PAR_R;
    case T_EOL:
        return I_DOLAR;
    case T_LESS:
        return I_LESS;
    case T_LESS_EQ:
        return I_LESS_EQ;
    case T_GREATER:
        return I_GREATER;
    case T_GREATER_EQ:
        return I_GREATER_EQ;
    case T_EQ:
        return I_EQ;
    case T_EQ_NIL:
        return I_EQ_NIL;
    case T_CONCAT:
        return I_CONCAT;
    case T_HASH:
        return I_HASH;
    default:
        return I_EROR;
    }
}

int reduce(Stack* stack, IdentType typevar, char *type)
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
        Stack_Push(stack, I_NON_TERM, data);
        //printf("(E) --> E \n");
        return 0;
    }
    
    while (topik->type != I_HALT) //<
    {
        topik = topik->next;
        cnt++;
    }
    if (cnt == 1 && (stack->top->type == I_ID || stack->top->type == I_NUMBER || stack->top->type == I_STRING || stack->top->type == I_INTEGER))
    {
        char* data = stack->top->data;
        tableItem_t *item;
        double num;
        typevar = stack->top->type;
        switch(stack->top->type){
            case I_ID:
                item = table_search_all(local_table, data);
                if(!item)
                    return ERR_SEM_DEF;
                printf("PUSHS ");
                print_var(item);
                *type = item->types[0];
                break;
            case I_NUMBER:
                num = strtod(data, NULL);
                printf("PUSHS float@%a\n", num);
                *type = 'N';
                break;
            case I_STRING:
                printf("PUSHS string@");
                print_ifjstring(data);
                *type = 'S';
                break;
            case I_INTEGER:
                printf("PUSHS int@%s\n", data);
                *type = 'I';
                break;
            case I_NULL:
                printf("PUSHS nil@nil\n");
                *type = 'n';
                break;
        }

        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack,I_NON_TERM,data);

        //printf("E: %s\n", data);
    }
    else if(cnt == 2 && stack->top->type == I_NON_TERM && stack->top->next->type == I_HASH)
    {
        if (typevar != I_STRING)
        {
            return 6;
        }
        
        char* data = Stack_Top_Data(stack); 
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack, I_NON_TERM, data);
        
        printf("POPS GF@op2\n");
        printf("STRLEN GF@op1 GF@op2\n");
        printf("PUSHS GF@op1\n");
        *type = 'I';
        typevar = I_INTEGER;
    }
    else if (cnt == 3 && stack->top->type == I_NON_TERM && stack->top->next->type >= I_PLUS && stack->top->next->type <= I_DIVIDE_INT && stack->top->next->next->type == I_NON_TERM)
    {
        if ((stack->top->next->type == I_DIVIDE || stack->top->next->type == I_DIVIDE_INT) && stack->top->data[0] == '0')
        {
            return 9;
        }
        char* data = NULL;
        //printf("E %d E --> E: \n", stack->top->next->type);

        switch(stack->top->next->type){
            case I_PLUS:
                printf("ADDS\n");
                break;
            case I_MINUS:
                printf("SUBS\n");
                break;
            case I_MULTIPLAY:
                printf("MULS\n");
                break;
            case I_DIVIDE:
                printf("DIVS\n");
                break;
            case I_DIVIDE_INT:
                printf("IDIVS\n");
                break;
        }

        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack, I_NON_TERM, data);

        //generace operace stack->top->next->data
    }
    else if (cnt == 3 && stack->top->type == I_NON_TERM && stack->top->next->type > I_DOLAR && stack->top->next->type < I_CONCAT && stack->top->next->next->type == I_NON_TERM)
    {
        switch(stack->top->next->type){
            case I_GREATER:
                printf("GTS\n");
                break;
            case I_LESS:
                printf("LTS\n");
                break;
            case I_EQ:
                printf("EQS\n");
                break;
            case I_EQ_NIL:
                printf("EQS\n");
                printf("NOTS\n");
                break;
            case I_LESS_EQ:
                printf("POPS GF@op2\n");
                printf("POPS GF@op1\n");
                printf("PUSHS GF@op1\n");
                printf("PUSHS GF@op2\n");
                printf("EQS\n");
                printf("POPS GF@op3\n");
                printf("PUSHS GF@op1\n");
                printf("PUSHS GF@op2\n");
                printf("LTS\n");
                printf("PUSHS GF@op3\n");
                printf("ORS\n");
                break;
            case I_GREATER_EQ:
                printf("POPS GF@op2\n");
                printf("POPS GF@op1\n");
                printf("PUSHS GF@op1\n");
                printf("PUSHS GF@op2\n");
                printf("EQS\n");
                printf("POPS GF@op3\n");
                printf("PUSHS GF@op1\n");
                printf("PUSHS GF@op2\n");
                printf("GTS\n");
                printf("PUSHS GF@op3\n");
                printf("ORS\n");
                break;
        }
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack, I_NON_TERM, NULL);
        *type = 'b';
    }
    else if (cnt == 3 && stack->top->type == I_NON_TERM && stack->top->next->type == I_CONCAT && stack->top->next->next->type == I_NON_TERM)
    {
        char* data = NULL;
        if (typevar != I_STRING)
        {
            return 6;
        }
        
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Pop(stack);
        Stack_Push(stack, I_NON_TERM, data);
        printf("POPS GF@op2\n");
        printf("POPS GF@op1\n");
        printf("CONCAT GF@op1 GF@op1 GF@op2\n");
        printf("PUSHS GF@op1\n");
        //printf("E .. E --> E: %s\n", stack->top->next->data);
        //generace operace stack->top->next->data
    }
    else 
    {
        return 6;
    }

    return 0;
}

int solvedExpression(token_t *token, char *type)
{
    Stack_Init(&s);
    Stack_Push(&s, I_DOLAR, NULL);
    int typevar = 0;
    bool end = false;
    char* Bdata = malloc(99);
    if (Bdata == NULL)
    {
        exit(99);
    }

    while (!end)
    {
        int a = stack_to_table(&s);
        int b;
        IdentType Btype;



        if (token->type == T_STRING)
        {
            if ((typevar == I_INTEGER || typevar == I_NUMBER)&& s.top->type!= I_HASH)
            {
               return 6;
            }
            if (s.top->type == I_PLUS || s.top->type == I_MINUS || s.top->type == I_MULTIPLAY || s.top->type == I_DIVIDE || s.top->type == I_DIVIDE_INT)
            {
                return 6;
            }
            typevar = I_STRING;
            
        }
        if (s.top->type == I_STRING && s.top->next->next->type != I_HASH)
        {
            if (token->type == T_ADD || token->type == T_SUB || token->type == T_MUL || token->type == T_DIV || token->type == T_DIV_INT)
            {
                return 6;
            }
        }
        
        if (token->type == T_CONCAT)
        {
            if (typevar != I_STRING)
            {
                return 6;
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
            if (s.top->type == I_STRING)
            {
                typevar = I_STRING;
            }
         }

        if(token->type == T_ID && (s.top->type == I_ID || s.top->type== I_NON_TERM || s.top->type == I_STRING || s.top->type == I_NUMBER || s.top->type == I_INTEGER)) 
        {
            break;
        }
        if (token->type == T_KW && (token->keyword == KW_DO || token->keyword == KW_THEN || token->keyword == KW_END || token->keyword == KW_FUNCTION || token->keyword == KW_GLOBAL || token->keyword == KW_IF || token->keyword == KW_LOCAL || token->keyword == KW_REQUIRE || token->keyword == KW_RETURN || token->keyword == KW_WHILE || token->keyword == KW_ELSE))
        {
            break;
        }



        if (token->type == T_EOL || token->type == T_EOF || token->type == T_COMMA)
        {
            b = 5;
            Btype = I_DOLAR;

            end = true;
        }
        else
        {
            b = get_index_to_table(token->type);
            Btype = TokentoIden(token->type);

            if (Btype == I_ID)
            {
                strcpy(Bdata, token->data);
            }
            else if(Btype == I_INTEGER)
            {          
                sprintf(Bdata, "%lld", token->integer);
            }
            else if (Btype == I_STRING)
            {         
                strcpy(Bdata,token->data);
            }
            else if (Btype == I_NUMBER)
            {
                sprintf(Bdata, "%f", token->number);
            }
            
        }
        int err;
        switch (Precedence_table[a][b])
        {
            case '=':
                Stack_Push(&s, Btype, Bdata);
                err = get_token(token);
                if (err)
                {
                    return err;
                }
                break;
            case '<':
                Stack_InsertBeforeNonTerm(&s, I_HALT, NULL); //<
                Stack_Push(&s, Btype, Bdata);
                err = get_token(token);
                if (err)
                {
                    return err;
                }
                
                break;
            case '>':
                err = reduce(&s, typevar, type);
                if(err)
                    return err;
                break;
            case 'D':
                err = reduce(&s, typevar, type);
                if(err)
                    return err;
                break;
            default:
                return 6;
        }
    }
    int a = stack_to_table(&s);
    while (a != 5)
    {
        int err = reduce(&s, typevar, type);
        if(err)
            return err;
        a = stack_to_table(&s);
    }
    Stack_Destroy(&s);
    
    return 0;
}

/**
 * @file parser.c
 * @author Martin Zmitko (xzmitk01@stud.fit.vutbr.cz)
 * @brief Implementace semantickeho a syntaktickeho parseru
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scanner.h"
#include "parser.h"
#include "stack.h"
#include "symtable.h"

int test_token(token_t *token){
    static int test_index = 0;
    token_t token_list[] = {{T_KW, KW_FUNCTION}, {T_ID, data: "foo"}, {T_PAR_L}, {T_ID, data: "bar"}, {T_COLON}, {T_KW, KW_NUMBER}, {T_PAR_R}, {T_COLON}, {T_KW, KW_STRING}, {T_KW, KW_RETURN}, {T_STRING, data: "test"}, {T_KW, KW_END}, {T_ID, data: "foo"}, {T_PAR_L}, {T_INTEGER, integer: 5}, {T_PAR_R}, {T_EOF}};
    *token = token_list[test_index++];
    return 0;
}

symtable_t *global_table;
tableItem_t *tItem;

char *string_create(int size){
    char *str = malloc(size);
    if(!str)
        exit(99);
    str[0] = 0;
    return str;
}

void string_append(char **str, char c){
    int len = strlen(*str);
    *str = realloc(*str, len + 2);
    if(*str == NULL)
        exit(99);
    (*str)[len] = c;
    (*str)[len + 1] = 0;
}

int check_types(char *types1, char *types2){
    int len1 = strlen(types1), len2 = strlen(types2);
    if(len1 != len2)
        return 1;
    for(int i = 0; i < len1; i++){
        if(types1[i] != types2[i] && types2[i] != 'n')
            return 1;
    }
    return 0;
}

int prog(token_t *token){
    PRINT_DEBUG;
    switch(token->type){
        case T_KW:
            switch(token->keyword){
                case KW_GLOBAL: //<prog> -> global ID : function ( <fdec_args> <f_types> <prog>
                    NEXT_CHECK_TYPE(token, T_ID);
                    if(table_search(global_table, token->data))
                        return ERR_SEM_DEF;
                    tItem = table_insert(global_table, token->data);
                    tItem->defined = false;
                    tItem->isFunc = true;
                    NEXT_CHECK_TYPE(token, T_COLON);
                    NEXT_CHECK_KW(token, KW_FUNCTION);
                    NEXT_CHECK_TYPE(token, T_PAR_L);
                    NEXT_TOKEN(token);
                    CALL_RULE(fdec_args, token);
                    CALL_RULE_EMPTY(f_types);
                    return prog(token);

                case KW_FUNCTION: //<prog> -> function ID ( <fdef_args> <f_types> <stat> <prog>
                    NEXT_CHECK_TYPE(token, T_ID);
                    char *params, *types;
                    bool checkTypes = false;
                    if((tItem = table_search(global_table, token->data))){
                        if(tItem->defined || !tItem->isFunc) //funkce jiz byla definovana, chyba
                            return ERR_SEM_DEF;
                        tItem->defined = true;
                    }
                    else{
                        tItem = table_insert(global_table, token->data);
                        tItem->defined = true;
                        tItem->isFunc = true;
                        checkTypes = true;
                        params = string_create(strlen(tItem->types) + 1);
                        types = string_create(strlen(tItem->types) + 1);
                        strcpy(params, tItem->params);
                        strcpy(types, tItem->types);
                    }
                    NEXT_CHECK_TYPE(token, T_PAR_L);
                    NEXT_TOKEN(token);
                    CALL_RULE(fdef_args, token);
                    CALL_RULE_EMPTY(f_types);
                    if(checkTypes){
                        if(strcmp(tItem->params, params) || strcmp(tItem->types, types))
                            return ERR_SEM_DEF; //typy parametru nebo navratovych hodnot nesedi
                        free(params);
                        free(types);
                    }
                    //GENERATE FUNCTION HEADER
                    //ADD NEW LOCAL SYMTABLE
                    CALL_RULE(stat, token);
                    return prog(token);

                case KW_REQUIRE: //<prog> -> require STRING <prog>
                    NEXT_CHECK_TYPE(token, T_STRING);
                    if(strcmp(token->data, "ifj21"))
                        return ERR_PARSE;
                    NEXT_TOKEN(token);
                    return prog(token);

                default:
                    return ERR_PARSE;
            }
            break;

        case T_ID: //<prog> -> ID ( <args> <prog>
            if(!(tItem = table_search(global_table, token->data)) || !tItem->isFunc)
                return ERR_SEM_DEF;
            NEXT_CHECK_TYPE(token, T_PAR_L);
            NEXT_TOKEN(token);
            CALL_RULE(args, token);
            //GENERATE CODE FOR FUNC CALL
            return prog(token);

        case T_EOF: //<prog> -> EOF
            return ERR_OK;

        default:
            return ERR_PARSE;
    }
}

int fdec_args(token_t *token){
    PRINT_DEBUG;
    if(token->type == T_PAR_R) //<fdec_args> -> )
        return ERR_OK;
    else{ //<fdec_args> -> <type> <fdec_args_n>
        CALL_RULE(type, token, true);
        return fdec_args_n(token);
    }
}

int fdec_args_n(token_t *token){
    PRINT_DEBUG;
    if(token->type == T_PAR_R) //<fdec_args_n> -> )
        return ERR_OK;
    else if(token->type == T_COMMA){ //<fdec_args_n> -> , <type> <fdec_args_n>
        NEXT_TOKEN(token);
        CALL_RULE(type, token, true);
        return fdec_args_n(token);
    }
    else
        return ERR_PARSE;
}

int fdef_args(token_t *token){
    PRINT_DEBUG;
    if(token->type == T_PAR_R) //<fdef_args> -> )
        return ERR_OK;
    else if(token->type == T_ID){ //<fdef_args> -> ID : <type> <fdef_args_n>
        NEXT_CHECK_TYPE(token, T_COLON);
        NEXT_TOKEN(token);
        CALL_RULE(type, token, true);
        return fdef_args_n(token);
    }
    else
        return ERR_PARSE;
}

int fdef_args_n(token_t *token){
    PRINT_DEBUG;
    if(token->type == T_PAR_R) //<fdef_args_n> -> )
        return ERR_OK;
    else if(token->type == T_COMMA){ //<fdef_args_n> -> , ID : <type> <fdef_args_n>
        NEXT_CHECK_TYPE(token, T_ID);
        NEXT_CHECK_TYPE(token, T_COLON);
        NEXT_TOKEN(token);
        CALL_RULE(type, token, true);
        return fdef_args_n(token);
    }
    else
        return ERR_PARSE;
}

int f_types(token_t *token, bool *empty){
    PRINT_DEBUG;
    if(token->type == T_COLON){ //<f_types> -> : <types>
        NEXT_TOKEN(token);
        return types(token, empty);
    }
    else{ //<f_types> -> e
        *empty = true;
        return ERR_OK;
    }
}

int types(token_t *token, bool *empty){ //<types> -> <type> <types_n>
PRINT_DEBUG;
    CALL_RULE(type, token, false);
    return types_n(token, empty);
}

int types_n(token_t *token, bool *empty){
    PRINT_DEBUG;
    if(token->type == T_COMMA){ //<types_n> -> , <type> <types_n>
        NEXT_TOKEN(token);
        CALL_RULE(type, token, false);
        return types_n(token, empty);
    }
    else{ //<types_n> -> e
        *empty = true;
        return ERR_OK;
    }
}

int args(token_t *token){
    PRINT_DEBUG;
    if(token->type == T_PAR_R) //<args> -> )
        return ERR_OK;
    else{ //<args> -> <term> <args_n>
        char *types = string_create(1);
        CALL_RULE(term, token, &types);
        return args_n(token, &types);
    }
}

int args_n(token_t *token, char **types){
    PRINT_DEBUG;
    if(token->type == T_PAR_R){ //<args_n> -> )
        if(check_types(tItem->params, *types))
            return ERR_SEM_PARAM;
        free(*types);
        return ERR_OK;
    }
    else if (token->type == T_COMMA){ //<args_n> -> , <term> <args_n>
        NEXT_TOKEN(token);
        CALL_RULE(term, token, types);
        return args_n(token, types);
    }
    else
        return ERR_PARSE;
}

int stat(token_t *token){
    PRINT_DEBUG;
    if(token->type == T_ID){
        tItem = table_search(global_table, token->data);
        if(!tItem)
            return ERR_SEM_DEF;
        if(tItem->isFunc){ //<stat> -> ID ( <args> <stat>
            NEXT_CHECK_TYPE(token, T_PAR_L);
            NEXT_TOKEN(token);
            CALL_RULE(args, token);
            //GENERATE CODE FOR FUNCTION CALL
            return stat(token);
        }
        else{ //<stat> -> <IDs> <EXPRs> <stat>
            CALL_RULE(IDs, token);
            CALL_RULE_EMPTY(EXPRs);
            return stat(token);
        }
    }
    else if(token->type == T_KW){
        switch(token->keyword){
            case KW_LOCAL:
                NEXT_CHECK_TYPE(token, T_ID);
                //TODO CHECK JESTLI JE V NEJVYSSIM LOKALNIM SYMTABLU, POTOM CHYBA
                tItem = table_insert(global_table, token->data);
                tItem->isFunc = false;
                NEXT_CHECK_TYPE(token, T_COLON);
                NEXT_TOKEN(token);
                CALL_RULE(type, token, false);
                //GENERATE VARIABLE DEFINITION
                if(token->type == T_EQ){
                    NEXT_TOKEN(token);
                    if(token->type == T_ID){
                        if(!(tItem = table_search(global_table/*NEBO LOKALNI*/, token->data)))
                            return ERR_SEM_DEF;
                        if(tItem->isFunc){ //<stat> -> local ID : <type> = ID ( <args> <stat>
                            NEXT_CHECK_TYPE(token, T_PAR_L);
                            NEXT_TOKEN(token);
                            CALL_RULE(args, token);
                            //GENERATE FUNCTION CALL AND SAVE RESULT
                        }
                        else{ //<stat> -> local ID : <type> = EXPR <stat> (starting with ID)
                            //PARSE EXPRESSION AND SAVE TO VARIABLE
                        }
                    }
                    else{ ////<stat> -> local ID : <type> = EXPR <stat> (starting with term)
                        //PARSE EXPRESSION AND SAVE TO VARIABLE
                    }
                }
                else //<stat> -> local ID : <type> <stat>
                    NEXT_TOKEN(token);
                return stat(token);

            case KW_IF: //<stat> -> if EXPR then <stat> else <stat> <stat>
                NEXT_TOKEN(token);
                //PARSE EXPRESSION
                //GENERATE IF HEADER
                NEXT_CHECK_KW(token, KW_THEN);
                NEXT_TOKEN(token);
                //ADD NEW LOCAL SYMTABLE
                CALL_RULE(stat, token);
                CHECK_KW(token, KW_ELSE);
                NEXT_TOKEN(token);
                //ADD NEW LOCAL SYMTABLE
                CALL_RULE(stat, token);
                return stat(token);

            case KW_WHILE: //<stat> -> while EXPR do <stat> <stat>
                NEXT_TOKEN(token);
                //PARSE EXPRESSION
                NEXT_CHECK_KW(token, KW_DO);
                NEXT_TOKEN(token);
                //GENERATE WHILE HEADER
                //ADD NEW LOCAL SYMTABLE
                CALL_RULE(stat, token);
                return stat(token);

            case KW_RETURN: //<stat> -> return <EXPRs> <stat>
                NEXT_TOKEN(token);
                CALL_RULE_EMPTY(EXPRs);
                return stat(token);

            case KW_END: //<stat> -> end
                //POP LOCAL STACKFRAME
                //GENERATE RETURN FOR WHATEVER BLOCK WE ARE IN (THIS SHIT WILL SUCK)
                return ERR_OK;

            default:
                return ERR_PARSE;
        }
    }
    else
        return ERR_PARSE;
}

int IDs(token_t *token){
    PRINT_DEBUG;
    if(token->type == T_ID){ //<IDs> -> ID <IDs_n>
        NEXT_TOKEN(token);
        return IDs_n(token);
    }
    else
        return ERR_PARSE;
}

int IDs_n(token_t *token){
    PRINT_DEBUG;
    if(token->type == T_EQ) //<IDs_n> -> =
        return ERR_OK;
    else if(token->type == T_COMMA){ //<IDs_n> -> , ID <IDs_n>
        NEXT_CHECK_TYPE(token, T_ID);
        NEXT_TOKEN(token);
        return IDs_n(token);
    }
    else
        return ERR_PARSE;
}

int EXPRs(token_t *token, bool *empty){
    PRINT_DEBUG;
    if(token->type == T_ID){ //<EXPRs> -> ID ( <args>
        NEXT_CHECK_TYPE(token, T_PAR_L);
        NEXT_TOKEN(token);
        CALL_RULE(args, token);
        return ERR_OK;
    }
    else{ //<EXPRs> -> EXPR <EXPRs_n>
        //PARSE EXPRESSION
        NEXT_TOKEN(token);
        return EXPRs_n(token, empty);
    }
}

int EXPRs_n(token_t *token, bool *empty){
    PRINT_DEBUG;
    if(token->type == T_COMMA){ //<EXPRs_n> -> , EXPR <EXPRs_n>
        NEXT_TOKEN(token);
        //PARSE EXPRESSION
        NEXT_TOKEN(token);
        return EXPRs_n(token, empty);
    }
    else{ //<EXPRs_n> -> e
        *empty = true;
        return ERR_OK;
    }
}

int type(token_t *token, bool params){
    PRINT_DEBUG;
    if(token->type != T_KW)
        return ERR_PARSE;
    char type;
    switch(token->keyword){ //TODO
        case KW_NUMBER:
            type = 'N';
            break;
        case KW_INTEGER:
            type = 'I';
            break;
        case KW_STRING:
            type = 'S';
            break;
        case KW_NIL:
            type = 'n';
            break;
        default:
            return ERR_PARSE;
    }
    if(params)
        string_append(&(tItem->params), type);
    else
        string_append(&(tItem->types), type);
    return ERR_OK;
}

int term(token_t *token, char **types){
    PRINT_DEBUG;
    tableItem_t *item;
    switch(token->type){ //TODO
        case T_ID:
            item = table_search(global_table/*TADY BUDE VYHLEDAVANI V LOKALNICH TABULKACH*/, token->data);
            if(!item)
                return ERR_SEM_DEF;
            if(item->isFunc)
                return ERR_PARSE;
            string_append(types, item->types[0]);
            //GENERATE CODE PUSH TO CALL STACK
            return ERR_OK;
        case T_NUMBER:
            string_append(types, 'N');
            //GENERATE CODE PUSH TO CALL STACK
            return ERR_OK;
        case T_INTEGER:
            string_append(types, 'I');
            //GENERATE CODE PUSH TO CALL STACK
            return ERR_OK;
        case T_STRING:
            string_append(types, 'S');
            //GENERATE CODE PUSH TO CALL STACK
            return ERR_OK;
        case T_KW:
            if(token->keyword == KW_NIL){
                string_append(types, 'n');
                //GENERATE CODE PUSH TO CALL STACK
                return ERR_OK;
            }
            else
                return ERR_PARSE;
        default:
            return ERR_PARSE; 
    }
}

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
#include "expression.h"

tableItem_t *tItem, *currFunc;
itemList_t *callList;
int whileID, ifID, depth;

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
    if(types1[0] == 'w')
        return 0;
    int len1 = strlen(types1), len2 = strlen(types2);
    if(len1 != len2)
        return 1;
    for(int i = 0; i < len1; i++){
        if(types1[i] != types2[i] && types2[i] != 'n')
            return 1;
    }
    return 0;
}

itemList_t *list_init(){
    itemList_t *list = malloc(sizeof(itemList_t));
    if(!list) 
        exit(99);
    list->item = NULL;
    list->next = NULL;
    return list;
}

void list_append(itemList_t *list, tableItem_t *item, tokenList_t *args){
    if(!list->item){
        list->item = item;
        list->args = args;
    }
    else{
        while(list->next)
            list = list->next;
        if(list->item){
            list->next = list_init();
            list->next->item = item;
            list->next->args = args;
        }
        else{
            list->item = item;
            list->args = args;
        }
    }
}

tokenList_t *t_list_init(){
    tokenList_t *list = malloc(sizeof(tokenList_t));
    if(!list) 
        exit(99);
    list->item = NULL;
    list->next = NULL;
    return list;
}

void t_list_append(tokenList_t **list, token_t *item){
    token_t *newToken = malloc(sizeof(token_t));
    if(!newToken) exit(99);
    memcpy(newToken, item, sizeof(token_t));
    tokenList_t *newList = t_list_init();
    newList->item = newToken;
    newList->next = *list;
    *list = newList;
}

void print_ifjstring(char *str){
    while(*str){
        if(*str <= 32 || *str == 35 || *str == 92)
            printf("\\%03d", (int)*str);
        else
            printf("%c", *str);
        str++;
    }
    printf("\n");
}

void print_var(tableItem_t *item){
    printf("LF@%s", item->key);
    if(item->id != -1)
        printf("$%d", item->id);
    printf("\n");
}

char *strrev(char *str)
{
      char *p1, *p2;
      if (*str == 0)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2){
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
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
                    tItem = table_insert(&global_table, token->data);
                    tItem->defined = false;
                    tItem->params = string_create(1);
                    tItem->types = string_create(1);
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
                        if(tItem->defined)
                            return ERR_SEM_DEF;
                        tItem->defined = true;
                        checkTypes = true;
                        params = string_create(strlen(tItem->types) + 1);
                        types = string_create(strlen(tItem->types) + 1);
                        strcpy(params, tItem->params);
                        strcpy(types, tItem->types);
                        tItem->params[0] = 0;
                        tItem->types[0] = 0;
                    }
                    else{
                        tItem = table_insert(&global_table, token->data);
                        tItem->defined = true;
                        tItem->params = string_create(1);
                        tItem->types = string_create(1);
                    }
                    NEXT_CHECK_TYPE(token, T_PAR_L);
                    NEXT_TOKEN(token);
                    table_list_insert(&local_table);
                    itemList_t *args = list_init();
                    CALL_RULE(fdef_args, token, args);
                    CALL_RULE_EMPTY(f_types);
                    if(checkTypes){
                        if(strcmp(tItem->params, params) || strcmp(tItem->types, types))
                            return ERR_SEM_DEF; //typy parametru nebo navratovych hodnot nesedi
                        free(params);
                        free(types);
                    }

                    printf("LABEL %s\n", tItem->key);
                    printf("CREATEFRAME\n");
                    itemList_t *i = args;
                    while(i && i->item){
                        printf("DEFVAR TF@%s\n", i->item->key);
                        printf("POPS TF@%s\n", i->item->key);
                        i = i->next;
                    }
                    printf("PUSHFRAME\n");

                    currFunc = tItem;
                    depth = 0;
                    CALL_RULE(stat, token);

                    return prog(token);

                case KW_REQUIRE: //<prog> -> require STRING <prog>
                    NEXT_CHECK_TYPE(token, T_STRING);
                    if(strcmp(token->data, "ifj21"))
                        return ERR_SEM_OTHER;
                    NEXT_TOKEN(token);
                    return prog(token);

                default:
                    return ERR_PARSE;
            }
            break;

        case T_ID: //<prog> -> ID ( <args> <prog>
            if(!(tItem = table_search(global_table, token->data)))
                return ERR_PARSE;
            NEXT_CHECK_TYPE(token, T_PAR_L);
            NEXT_TOKEN(token);
            tokenList_t *list = t_list_init();
            CALL_RULE(args, token, &list);
            
            if(!callList)
                callList = list_init();
            list_append(callList, tItem, list); //TODO SOMETHIGN WITH THIS SHIT

            return prog(token);

        case T_EOF:
            printf("LABEL $CALLS$\n");
            while(callList){
                itemList_t *item = callList;
                callList = callList->next;

                char *params = string_create(1);
                while(item->args && item->args->item){
                    int err = term(item->args->item, &params);
                    if(err) return err;
                    item->args = item->args->next;
                }
                params = strrev(params);
                
                if(check_types(tItem->params, params))
                    return ERR_SEM_PARAM;
                if(tItem->params[0] == 'w')
                    printf("PUSHS int@%ld\n", strlen(params));
                printf("CALL %s\n", item->item->key);
                free(item);
            }
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

int fdef_args(token_t *token, itemList_t *args){
    PRINT_DEBUG;
    if(token->type == T_PAR_R) //<fdef_args> -> )
        return ERR_OK;
    else if(token->type == T_ID){ //<fdef_args> -> ID : <type> <fdef_args_n>
        tableItem_t *func = tItem; //k itemu pro funkci se budeme vracet
        if(table_search(local_table->table, token->data))
            return ERR_SEM_DEF;
        tItem = table_insert(&(local_table->table), token->data);
        tItem->types = string_create(1);
        tItem->id = -1;
        list_append(args, tItem, NULL);
        NEXT_CHECK_TYPE(token, T_COLON);
        NEXT_TOKEN(token);
        if(type(token, false))
            return ERR_PARSE;
        tItem = func;
        CALL_RULE(type, token, true);
        return fdef_args_n(token, args);
    }
    else
        return ERR_PARSE;
}

int fdef_args_n(token_t *token, itemList_t *args){
    PRINT_DEBUG;
    if(token->type == T_PAR_R) //<fdef_args_n> -> )
        return ERR_OK;
    else if(token->type == T_COMMA){ //<fdef_args_n> -> , ID : <type> <fdef_args_n>
        tableItem_t *func = tItem; //k itemu pro funkci se budeme vracet
        NEXT_CHECK_TYPE(token, T_ID);
        tItem = table_insert(&(local_table->table), token->data); //TODO CHECK JESTLI SE PARAMETR NEOPAKUJE
        tItem->types = string_create(1);
        list_append(args, tItem, NULL);
        NEXT_CHECK_TYPE(token, T_COLON);
        NEXT_TOKEN(token);
        if(type(token, false))
            return ERR_PARSE;
        tItem = func;
        CALL_RULE(type, token, true);
        return fdef_args_n(token, args);
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

int args(token_t *token, tokenList_t **list){
    PRINT_DEBUG;
    if(token->type == T_PAR_R) //<args> -> )
        return ERR_OK;
    else{ //<args> -> <term> <args_n>
        t_list_append(list, token);
        NEXT_TOKEN(token);
        return args_n(token, list);
    }
}

int args_n(token_t *token, tokenList_t **list){
    PRINT_DEBUG;
    if(token->type == T_PAR_R){ //<args_n> -> )
        return ERR_OK;
    }
    else if (token->type == T_COMMA){ //<args_n> -> , <term> <args_n>
        NEXT_TOKEN(token);
        t_list_append(list, token);
        NEXT_TOKEN(token);
        return args_n(token, list);
    }
    else
        return ERR_PARSE;
}

int stat(token_t *token){
    PRINT_DEBUG;
    if(token->type == T_ID){
        tItem = table_search(global_table, token->data);
        if(!tItem){ //<stat> -> <IDs> <EXPRs> <stat>
            itemList_t *list = list_init();
            size_t count = 0;
            CALL_RULE(IDs, token, list); //itemy kazde promenne se ulozi do listu
            char *types = NULL;
            CALL_RULE_EMPTY(EXPRs, &types);
            int i = 0;
            if(strlen(types) < count)
                return ERR_SEM_ASSIGN; //MAYBE??
            while(list){
                if(list->item->types[0] != types[i] && types[i] != 'n')
                    return ERR_SEM_PARAM; //TODO INT TO NUM CONVERSION
                printf("POPS ");
                print_var(list->item);
                i++;
                list = list->next;
            }
            return stat(token);
        }
        else{ //<stat> -> ID ( <args> <stat>
            NEXT_CHECK_TYPE(token, T_PAR_L);
            NEXT_TOKEN(token);

            tokenList_t *list = t_list_init();
            CALL_RULE(args, token, &list);

            char *params = string_create(1);
            while(list && list->item){
                int err = term(list->item, &params);
                if(err) return err;
                list = list->next;
            }
            if(check_types(tItem->params, params))
                return ERR_SEM_PARAM;
            if(tItem->params[0] == 'w')
                printf("PUSHS int@%ld\n", strlen(params));
            
            printf("CALL %s\n", tItem->key);

            return stat(token);
        }
    }
    else if(token->type == T_KW){
        char exprType;
        switch(token->keyword){
            case KW_LOCAL:
                NEXT_CHECK_TYPE(token, T_ID);
                if(table_search(global_table, token->data))
                    return ERR_SEM_DEF;
                if(table_search_first(local_table, token->data))
                    return ERR_SEM_DEF;
                tItem = table_insert(&(local_table->table), token->data);
                tItem->types = string_create(1);
                tItem->id = depth;
                NEXT_CHECK_TYPE(token, T_COLON);
                NEXT_TOKEN(token);
                CALL_RULE(type, token, false);
                
                tableItem_t *item = tItem;
                printf("DEFVAR ");
                print_var(item);

                if(token->type == T_ASSIGN){
                    NEXT_TOKEN(token);
                    if(token->type == T_ID){
                        if((tItem = table_search(global_table, token->data))){ //<stat> -> local ID : <type> = ID ( <args> <stat>
                            NEXT_CHECK_TYPE(token, T_PAR_L);
                            NEXT_TOKEN(token);
                            
                            tokenList_t *list = t_list_init();
                            CALL_RULE(args, token, &list);

                            char *params = string_create(1);
                            while(list && list->item){
                                int err = term(list->item, &params);
                                if(err) return err;
                                list = list->next;
                            }
                            if(check_types(tItem->params, params))
                                return ERR_SEM_PARAM;
                            if(check_types(item->types, tItem->types))
                                return ERR_SEM_ASSIGN;

                            printf("CALL %s\n", tItem->key); //TODO CHECK TYPES COUNT IS 1
                            printf("POPS ");
                            print_var(item);
                        }
                        else if((tItem = table_search_all(local_table, token->data))){ //<stat> -> local ID : <type> = EXPR <stat> (starting with ID)
                            char exprType;
                            CALL_EXPR(token, &exprType);
                            if(item->types[0] != exprType && exprType != 'n')
                                return ERR_SEM_ASSIGN;
                            printf("POPS ");
                            print_var(item);
                        }
                        else
                            return ERR_SEM_DEF;
                    }
                    else{ ////<stat> -> local ID : <type> = EXPR <stat> (starting with term)
                        char exprType;
                        CALL_EXPR(token, &exprType);
                        if(item->types[0] != exprType && exprType != 'n')
                            return ERR_SEM_ASSIGN;
                        printf("POPS ");
                        print_var(item);
                    }
                }

                return stat(token);

            case KW_IF: //<stat> -> if EXPR then <stat> else <stat> <stat>
                NEXT_TOKEN(token);
                CALL_EXPR(token, &exprType);
                if(exprType == 'n'){
                    printf("JUMP %s$ELSE$%d\n", currFunc->key, ifID);
                }
                else if(exprType == 'b'){
                    printf("PUSHS bool@false\n");
                    printf("JUMPIFEQS %s$ELSE$%d\n", currFunc->key, ifID);
                }

                CHECK_KW(token, KW_THEN);
                NEXT_TOKEN(token);
                table_list_insert(&local_table);
                
                depth++; ifID++;
                CALL_RULE(stat, token);
                ifID--;
                printf("JUMP %s$ENDIF$%d\n", currFunc->key, ifID);

                table_list_insert(&local_table);

                printf("LABEL %s$ELSE$%d\n", currFunc->key, ifID);
                depth++; ifID++;
                CALL_RULE(stat, token);
                ifID--;
                printf("LABEL %s$ENDIF$%d\n", currFunc->key, ifID);

                return stat(token);

            case KW_WHILE: //<stat> -> while EXPR do <stat> <stat>
                NEXT_TOKEN(token);
                table_list_insert(&local_table);
                printf("LABEL %s$WHILE$%d\n", currFunc->key, whileID);
                CALL_EXPR(token, &exprType);
                if(exprType == 'n'){
                    printf("JUMP %s$WHILE_END$%d\n", currFunc->key, whileID);
                }
                else if(exprType == 'b'){
                    printf("PUSHS bool@false\n");
                    printf("JUMPIFEQS %s$WHILE_END$%d\n", currFunc->key, whileID);
                }
                whileID++;
                CHECK_KW(token, KW_DO);
                NEXT_TOKEN(token);
                depth++;
                CALL_RULE(stat, token);

                whileID--;
                printf("JUMP %s$WHILE$%d\n", currFunc->key, whileID);
                printf("LABEL %s$WHILE_END$%d\n", currFunc->key, whileID);

                return stat(token);

            case KW_RETURN: //<stat> -> return <EXPRs> <stat>
                NEXT_TOKEN(token);
                char *types = NULL;
                CALL_RULE_EMPTY(EXPRs, &types);
                while(strlen(types) < strlen(currFunc->types))
                    printf("PUSHS nil@nil\n");
                if(strlen(types) > strlen(currFunc->types))
                    return ERR_SEM_PARAM;
                printf("POPFRAME\n");
                printf("RETURN\n\n");
                return stat(token);

            case KW_END: case KW_ELSE: //<stat> -> end; <stat> -> else
                table_list_delete(&local_table);
                if(depth == 0){
                    for(int i = 0; i < (int)strlen(currFunc->types); i++)
                        printf("PUSHS nil@nil\n");
                    printf("POPFRAME\n");
                    printf("RETURN\n\n");
                }
                else
                    depth--;
                return ERR_OK;

            default:
                return ERR_PARSE;
        }
    }
    else
        return ERR_PARSE;
}

int IDs(token_t *token, itemList_t *list){
    PRINT_DEBUG;
    if(token->type == T_ID){ //<IDs> -> ID <IDs_n>
        tItem = table_search_all(local_table, token->data);
        if(!tItem)
            return ERR_SEM_DEF;
        list_append(list, tItem, NULL);
        NEXT_TOKEN(token);
        return IDs_n(token, list);
    }
    else
        return ERR_PARSE;
}

int IDs_n(token_t *token, itemList_t *list){
    PRINT_DEBUG;
    if(token->type == T_ASSIGN) //<IDs_n> -> =
        return ERR_OK;
    else if(token->type == T_COMMA){ //<IDs_n> -> , ID <IDs_n>
        NEXT_CHECK_TYPE(token, T_ID);
        tItem = table_search_all(local_table, token->data);
        if(!tItem)
            return ERR_SEM_DEF;
        list_append(list, tItem, NULL);
        NEXT_TOKEN(token);
        return IDs_n(token, list);
    }
    else
        return ERR_PARSE;
}

int EXPRs(token_t *token, bool *empty, char **types){
    PRINT_DEBUG;
    if(token->type == T_ID){
        tItem = table_search(global_table, token->data);
        if(tItem){ ////<EXPRs> -> ID ( <args>
            NEXT_CHECK_TYPE(token, T_PAR_L);
            NEXT_TOKEN(token);
            
            tokenList_t *list = t_list_init();
            CALL_RULE(args, token, &list);

            char *params = string_create(1);
            while(list && list->item){
                int err = term(list->item, &params);
                if(err) return err;
                list = list->next;
            }
            if(check_types(tItem->params, params))
                return ERR_SEM_PARAM;
            

            printf("CALL %s\n", tItem->key);
            *types = tItem->types;
            *empty = true;
            return ERR_OK;
        }
    }
    //<EXPRs> -> EXPR <EXPRs_n>
    *types = string_create(1);
    char exprType = 0;
    CALL_EXPR(token, &exprType);
    string_append(types, exprType);
    return EXPRs_n(token, empty, types);
}

int EXPRs_n(token_t *token, bool *empty, char **types){
    PRINT_DEBUG;
    if(token->type == T_COMMA){ //<EXPRs_n> -> , EXPR <EXPRs_n>
        NEXT_TOKEN(token);
        char exprType = 0;
        CALL_EXPR(token, &exprType);
        string_append(types, exprType);
        return EXPRs_n(token, empty, types);
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
    switch(token->keyword){
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
    //Pridat typ na konec stringu
    int len = strlen(params ? tItem->params : tItem->types);
    char *str = realloc(params ? tItem->params : tItem->types, len + 2);
    if(str == NULL)
        exit(99);
    str[len] = type;
    str[len + 1] = 0;
    if(params)
        tItem->params = str;
    else
        tItem->types = str;
    return ERR_OK;
}

int term(token_t *token, char **types){
    PRINT_DEBUG;
    tableItem_t *item;
    switch(token->type){
        case T_ID:
            item = table_search_all(local_table, token->data);
            if(!item)
                return ERR_SEM_DEF;
            string_append(types, item->types[0]);
            printf("PUSHS ");
            print_var(item);
            return ERR_OK;
        case T_NUMBER:
            string_append(types, 'N');
            printf("PUSHS float@%a\n", token->number);
            return ERR_OK;
        case T_INTEGER:
            string_append(types, 'I');
            printf("PUSHS int@%lld\n", token->integer);
            return ERR_OK;
        case T_STRING:
            string_append(types, 'S');
            printf("PUSHS string@");
            print_ifjstring(token->data);
            return ERR_OK;
        case T_KW:
            if(token->keyword == KW_NIL){
                string_append(types, 'n');
                printf("PUSHS nil@nil\n");
                return ERR_OK;
            }
            else
                return ERR_PARSE;
        default:
            return ERR_PARSE; 
    }
}

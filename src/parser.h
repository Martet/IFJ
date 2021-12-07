/**
 * @file parser.h
 * @author Martin Zmitko (xzmitk01@stud.fit.vutbr.cz)
 * @brief Deklarace funkci semantickeho a syntaktickeho analyzatoru
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "scanner.h"
#include "symtable.h"
#include "expression.h"

/**
 * @brief Navratovy kod pro chyby prekladace
 */
typedef enum errCode {
    ERR_OK,
    ERR_LEX,
    ERR_PARSE,
    ERR_SEM_DEF,
    ERR_SEM_ASSIGN,
    ERR_SEM_PARAM,
    ERR_SEM_TYPE,
    ERR_SEM_OTHER,
    ERR_NIL,
    ERR_ZERO
} ErrCode;

typedef struct ItemList {
    struct ItemList *next;
    tableItem_t *item;
} itemList_t;

tableItem_t *global_table;
tableList_t *local_table;

/**
 * @brief Makro ulozi do promenne token novy token, pri chybe vrati chybovy kod lexeru
 * @param token Ukazatel na token
 */
#define NEXT_TOKEN(token)                                           \
    do{                                                             \
        if(get_token(token))                                        \
            return ERR_LEX;                                         \
        while((token)->type == T_EOL)                                 \
            if(get_token(token))                                    \
                return ERR_LEX;                                     \
    } while(0)

/**
 * @brief Makro nacte dalsi token, zkontroluje typ, pokud nesedi, vrati chybu v parseru
 * @param token Ukazatel na token
 * @param type Typ pro kontrolu tokenu
 */
#define NEXT_CHECK_TYPE(token, tp)                                  \
    do{                                                             \
        NEXT_TOKEN(token);                                          \
        if((token)->type != (tp))                                   \
            return ERR_PARSE;                                       \
    } while(0)                                                      \

/**
 * @brief Makro nacte dalsi token, zkontroluje jestli je typ keyword a typ keywordu, pokud nesedi, vrati chybu v parseru
 * @param token Ukazatel na token
 * @param type Typ keywordu pro kontrolu
 */
#define NEXT_CHECK_KW(token, kw)                                    \
    do{                                                             \
        NEXT_TOKEN(token);                                          \
        if(!((token)->type == T_KW && (token)->keyword == (kw)))    \
            return ERR_PARSE;                                       \
    } while(0) 

/**
 * @brief Makro zavola funkci pravidla rule, pri chybe vrati chybovy kod pravidla
 * @param rule Pravidlo pro zavolani
 * @param __VA_ARGS__ argumenty pravidla
 */
#define CALL_RULE(rule, ...)                                        \
    do{                                                             \
        int err = rule(__VA_ARGS__);                                \
        if(err) return err;                                         \
        NEXT_TOKEN(token);                                          \
    } while(0)

/**
 * @brief Makro zkontroluje jestli je typ keyword a typ keywordu, pokud nesedi, vrati chybu v parseru
 * @param token Ukazatel na token
 * @param type Typ keywordu pro kontrolu
 */
#define CHECK_KW(token, kw)                                         \
    do{                                                             \
        if(!((token)->type == T_KW && (token)->keyword == (kw)))    \
            return ERR_PARSE;                                       \
    } while(0) 

#define CALL_RULE_EMPTY(rule, ...)                                  \
    do{                                                             \
        bool empty = false;                                         \
        int err = rule(token, &empty, ## __VA_ARGS__);              \
        if(err) return err;                                         \
        if(!empty)                                                  \
            NEXT_TOKEN(token);                                      \
    } while(0)

#define CALL_EXPR(token)                                            \
    do{                                                             \
        int err = solvedExpression(token);                          \
        if(err) return err;                                         \
        if((token)->type == T_EOL) NEXT_TOKEN(token);               \
    } while(0)

//#define PRINT_DEBUG printf("                Line %d: %s (%s)\n", __LINE__, __func__, token->data)
#define PRINT_DEBUG

/**
 * @brief Vytvori dynamicky alokovany string
 * 
 * @param int Velikost noveho stringu
 * @return Ukazatel na vytvoreny string
 */
char *string_create(int size);

/**
 * @brief Prida znak na konec dynamickeho stringu
 * 
 * @param str Ukazatel na ukazatel na dynamicky alokovany string
 * @param c Znak pro pridani
 */
void string_append(char **str, char c);

/**
 * @brief Kontrola dvou seznamu datovych typu, pri neshode muze byt typ z types2 nil
 * 
 * @param types1 Seznam typu ke kontrole
 * @param types2 Seznam typu ke kontrole
 * @return 0 pri shode, 1 pri neshode
 */
int check_types(char *types1, char *types2);

/**
 * @brief Inicializace linked listu symtable itemu
 * 
 * @return Ukazatel na nove vytvoreny list
 */
itemList_t *list_init();

/**
 * @brief Pridani polozky na konec listu
 * 
 * @param list List pro upravu
 * @param item Polozka pro pridani
 */
void list_append(itemList_t *list, tableItem_t *item);

/**
 * @brief Implementace pravidla <prog>, startovni bod parseru
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int prog(token_t *token);

/**
 * @brief Implementace pravidla <fdec_args>
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int fdec_args(token_t *token);

/**
 * @brief Implementace pravidla <fdec_args_n>
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int fdec_args_n(token_t *token);

/**
 * @brief Implementace pravidla <fdef_args>
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int fdef_args(token_t *token, itemList_t *args);

/**
 * @brief Implementace pravidla <fdef_args_n>
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int fdef_args_n(token_t *token, itemList_t *args);

/**
 * @brief Implementace pravidla <f_types>
 * 
 * @param token Dalsi token pro zpracovani
 * @param empty Navraceni po epsilon pravidlu (kdyz true, necist dalsi token)
 * @return int Chybovy kod
 */
int f_types(token_t *token, bool *empty);

/**
 * @brief Implementace pravidla <types>
 * 
 * @param token Dalsi token pro zpracovani
 * @param empty Navraceni po epsilon pravidlu (kdyz true, necist dalsi token)
 * @return int Chybovy kod
 */
int types(token_t *token, bool *empty);

/**
 * @brief Implementace pravidla <types_n>
 * 
 * @param token Dalsi token pro zpracovani
 * @param empty Navraceni po epsilon pravidlu (kdyz true, necist dalsi token)
 * @return int Chybovy kod
 */
int types_n(token_t *token, bool *empty);

/**
 * @brief Implementace pravidla <args>
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int args(token_t *token);

/**
 * @brief Implementace pravidla <args_n>
 * 
 * @param token Dalsi token pro zpracovani
 * @param types Ukazatel na dynamicky string pro kontrolu parametru
 * @return int Chybovy kod
 */
int args_n(token_t *token, char **types);

/**
 * @brief Implementace pravidla <stat>
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int stat(token_t *token);

/**
 * @brief Implementace pravidla <IDs>
 * 
 * @param token Dalsi token pro zpracovani
 * @param list Ukazatel na list, kam se budou pridavat promenne
 * @return int Chybovy kod
 */
int IDs(token_t *token, itemList_t *list);

/**
 * @brief Implementace pravidla <IDs_n>
 * 
 * @param token Dalsi token pro zpracovani
 * @param list Ukazatel na list, kam se budou pridavat promenne
 * @return int Chybovy kod
 */
int IDs_n(token_t *token, itemList_t *list);

/**
 * @brief Implementace pravidla <EXPRs>
 * 
 * @param token Dalsi token pro zpracovani
 * @param empty Navraceni po epsilon pravidlu (kdyz true, necist dalsi token)
 * @param types Dynamicky string s typy vyrazu
 * @return int Chybovy kod
 */
int EXPRs(token_t *token, bool *empty, char **types);

/**
 * @brief Implementace pravidla <EXPRs_n>
 * 
 * @param token Dalsi token pro zpracovani
 * @param empty Navraceni po epsilon pravidlu (kdyz true, necist dalsi token)
 * @param types Dynamicky string s typy vyrazu
 * @return int Chybovy kod
 */
int EXPRs_n(token_t *token, bool *empty, char **types);

/**
 * @brief Implementace pravidla <type>
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int type(token_t *token, bool params);

/**
 * @brief Implementace pravidla <term>
 * 
 * @param token Dalsi token pro zpracovani
 * @param types Dynamicky string se seznamem typu pro kontrolu
 * @return int Chybovy kod
 */
int term(token_t *token, char **types);

#endif

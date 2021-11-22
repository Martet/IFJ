/**
 * @file parser.h
 * @author Martin Zmitko (xzmitk01@stud.fit.vutbr.cz)
 * @brief Deklarace funkci semantickeho a syntaktickeho analyzatoru
 */

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"

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

//Tady budou struktury pro ulozeni dat funkce (typ, argumenty...)
//TODO
typedef enum funcType {
    FUNC_DEF,
    FUNC_DEC,
    FUNC_CALL
} FuncType;

typedef struct funcData {
    FuncType type;
    int argc;
    
} funcData_t;

/**
 * @brief Makro ulozi do promenne token novy token, pri chybe vrati chybovy kod lexeru
 * @param token Ukazatel na token
 */
#define NEXT_TOKEN(token)                                           \
    do{                                                             \
        if(get_token(token))                                        \
            return ERR_LEX;                                         \
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
        if(err)                                                     \
            return err;                                             \
        NEXT_TOKEN(token);                                          \
    } while(0)

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
int fdef_args(token_t *token);

/**
 * @brief Implementace pravidla <fdef_args_n>
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int fdef_args_n(token_t *token);

/**
 * @brief Implementace pravidla <f_types>
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int f_types(token_t *token);

/**
 * @brief Implementace pravidla <types>
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int types(token_t *token);

/**
 * @brief Implementace pravidla <args>
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int args(token_t *token);

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
 * @return int Chybovy kod
 */
int IDs(token_t *token);

/**
 * @brief Implementace pravidla <EXPRs>
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int EXPRs(token_t *token);

/**
 * @brief Implementace pravidla <type>
 * 
 * @param token Dalsi token pro zpracovani
 * @return int Chybovy kod
 */
int type(token_t *token);

#endif

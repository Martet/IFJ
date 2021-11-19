/**
 * @file parser.h
 * @author Martin Zmitko (xzmitk01@stud.fit.vutbr.cz)
 * @brief Deklarace funkci semantickeho a syntaktickeho analyzatoru
 */

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"

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

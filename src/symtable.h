/**
 * @file symtable.h
 * @author Dominik Klon (xklond00@stud.fit.vutbr.cz)
 * @author Martin Zmitko (xzmitk01@stud.fit.vutbr.cz)
 * @brief Implementace tabulky symbolu pomoci binarniho vyhledavaciho stromu
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>

typedef struct TableItem {
    bool defined;
    bool isFunc;
    char *types;
    char *params;
    char *key;
    struct TableItem *lptr; 
    struct TableItem *rptr;
} tableItem_t;

typedef struct TableList {
    tableItem_t *table;
    struct TableList *next;
} tableList_t;

/**
 * @brief Inicializace nove polozky tabulky symbolu
 * 
 * @param key Klic nove polozky
 * @return ukazatel na root inicializovane tabulky
 */
tableItem_t *node_init(char *key);

/**
 * @brief Vyhledani polozky v tabulce symbolu
 * 
 * @param table Ukazatel na tabulku, ve ktere se bude vyhledavat
 * @param key Hledany klic
 * @return Ukazatel na nalezenou polozku, NULL kdyz nenalezeno
 */
tableItem_t *table_search(tableItem_t *table, char *key);

/**
 * @brief Vlozeni polozky to tabulky symbolu
 * 
 * @param table Ukazatel na tabulku
 * @param key Klic nove polozky
 * @return Ukazatel na vlozenou polozku
 */
tableItem_t *table_insert(tableItem_t **table, char *key);

/**
 * @brief Inicializace nove polozky seznamu tabulek symbolu
 * 
 * @return ukazatel na novou polozku seznamu
 */
tableList_t *table_list_init();

/**
 * @brief Vlozeni tabulky symbolu na zacatek seznamu
 * 
 * @param list Seznam tabulek
 */
void table_list_insert(tableList_t **list);

/**
 * @brief Odstraneni tabulky symbolu ze zacatku seznamu
 * 
 * @param list Seznam tabulek
 */
void table_list_delete(tableList_t **list);

/**
 * @brief Vyhledani polozky v prvni tabulce seznamu
 * 
 * @param list seznam tabulek
 * @param key klic pro hledani
 * @return tableItem_t* nalezena polozka, prip. NULL
 */
tableItem_t *table_search_first(tableList_t *list, char *key);

/**
 * @brief Vyhledani polozky postupne ve vsech tablukach seznamu
 * 
 * @param list seznam tabulek
 * @param key klic pro hledani
 * @return tableItem_t* nalezena polozka, prip. NULL
 */
tableItem_t *table_search_all(tableList_t *list, char *key);

#endif

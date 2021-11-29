#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>

typedef struct TableItem {
    bool defined;
    bool isFunc;
    char *types;
    char *params;
    char *key;
} tableItem_t;

typedef struct Symtable symtable_t;

/**
 * @brief Vyhledani polozky v tabulce symbolu
 * 
 * @param table Ukazatel na tabulku, ve ktere se bude vyhledavat
 * @param key Hledany klic
 * @return Ukazatel na nalezenou polozku, NULL kdyz nenalezeno
 */
tableItem_t *table_search(symtable_t *table, char *key);

/**
 * @brief Vlozeni polozky to tabulky symbolu
 * 
 * @param table Ukazatel na tabulku
 * @param key Klic nove polozky
 * @return Ukazatel na vlozenou polozku
 */
tableItem_t *table_insert(symtable_t *table, char *key);

#endif

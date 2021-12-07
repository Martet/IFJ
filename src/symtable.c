/**
 * @file symtable.c
 * @author Dominik Klon (xklond00@stud.fit.vutbr.cz)
 * @author Martin Zmitko (xzmitk01@stud.fit.vutbr.cz)
 * @brief Implementace tabulky symbolu pomoci binarniho vyhledavaciho stromu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

tableItem_t *node_init(char *key){
	tableItem_t *node = malloc(sizeof(tableItem_t));
	if(!node)
		exit(99);
	node->lptr = NULL;
	node->rptr = NULL;
	node->key = key;
	return node;
}

tableItem_t *table_insert(tableItem_t **table, char *key){
	if (*table == NULL){
		*table = node_init(key);
		return *table;
	}
	else{
		int cmp = strcmp(key, (*table)->key);
		if(cmp < 0)
			return table_insert(&((*table)->lptr), key);
		else if(cmp > 0)
			return table_insert(&((*table)->rptr), key);
		else
			return *table;
	}
}

tableItem_t *table_search(tableItem_t *table, char *key){
	if (table == NULL)
		return NULL;
 	else{
		int cmp = strcmp(key, table->key);
		if(cmp == 0)
			return table;
		else if(cmp < 0)
			return table_search(table->lptr, key);
		else
			return table_search(table->rptr, key);
	}
}

tableList_t *table_list_init(){
	tableList_t *list = malloc(sizeof(tableList_t));
	if(list == NULL)
		exit(99);
	list->table = NULL;
	list->next = NULL;
	return list;
}

void table_list_insert(tableList_t **list){
	if(*list == NULL){
		*list = table_list_init();
	}
	else{
		tableList_t *l = *list;
		*list = table_list_init();
		(*list)->next = l;
	}
}

void table_list_delete(tableList_t **list){
	tableList_t *next = (*list)->next;
	free(*list);
	*list = next;
}

tableItem_t *table_search_first(tableList_t *list, char *key){
	return table_search(list->table, key);
}

tableItem_t *table_search_all(tableList_t *list, char *key){
	while(list){
		tableItem_t *result = table_search(list->table, key);
		if(result)
			return result;
		list = list->next;
	}
	return NULL;
}

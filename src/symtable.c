
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"
/* 
tableItem_t item{
	defined=false,
	isFunc=false,
	->types=NULL;
}; */

/* tableItem_t *table_init(symtable_t **table){
	table=NULL;
} */
void table_values_init(symtable_t *table){
	table->item.types=NULL;
	table->item.params=NULL;
	table->item.key=NULL;
	table->item.defined=false;
	table->item.isFunc=false;

	table->lptr=NULL;
	table->rptr=NULL;

}
tableItem_t *table_insert(symtable_t *table, char *key){

/////////////////////// KOD Z IALU

	if (table == NULL) // konec rekurze
	{
		table = (symtable_t*) malloc(sizeof(struct symtable_t*));
		if (table!=NULL)
		{
			strcpy(table->item.key, key);
			
			table->lptr = NULL;
			table->rptr = NULL;
		}
		return &(table->item);
	}
	else{
		int cmp;
		cmp=strcmp(key,table->item.key);

		if (cmp<0)
		{
			table_insert(table->lptr, key);
		}
		else if (cmp>0)
		{
			table_insert(table->rptr, key);
		}
		else{
			table->item.key = key;
		}
		return NULL;
	}
//////////////////////  KOD Z IALU
}// end of table_insert function

tableItem_t *table_search(symtable_t *table, char *key){
	int cmp = strcmp(key,table->item.key);
	if (table == NULL)
	{
		return NULL;
	} 
 	else{
		if (cmp==0)
		{
			return &(table->item);
		}
		else if (cmp>0)
		{
			return table_search(table->lptr, key);
		}
		else{
			return table_search(table->rptr, key);
		}
	}//if table==NULL
}
// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9

#include <string.h>
#include <stdlib.h>
#include "htable.h"

//finds and removes the item from the hash table
void htab_remove(htab_t *t, const char *key){

	//hamdle error state
	if (t == NULL || key == NULL)
		fatal_error("Wrong parameters: htab_remove");

	htab_listitem *temp;
	//index of key in the hash table
	unsigned int i = t->hash_fun_ptr(key, t->n);
	for (htab_listitem *item = *(t->htab_size + i);	item != NULL; item = temp){
		//store pointer to next item
		temp = item->next;
		//free allocated space of item if the keys are identical
		if (!strcmp(item->key, key)){
			free(item->key);
			free(item);
			item = NULL;
			*(t->htab_size + i) = NULL;
			return;
		}
	}
}

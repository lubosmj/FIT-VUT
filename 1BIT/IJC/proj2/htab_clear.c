// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9

#include <stdlib.h>
#include "htable.h"

//clears all items in hash table
void htab_clear(htab_t *t){

	//handle error state
	if (t == NULL)
		fatal_error("Wrong parameter: htab_clear");

	htab_listitem *temp;
	//iterates through the hash table to remove every stored item
	for (unsigned int i = 0; i < t->n; i++){
		for (htab_listitem *item = *(t->htab_size + i); item != NULL; 
				item = temp){
			//store pointer to next item 
			temp = item->next;
			//free allocated space of the item
			free(item->key);
			free(item);
		}
	}
}

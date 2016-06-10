// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9

#include <stdio.h>
#include "htable.h"

//calls function (print) for each item in hash table
void htab_foreach(htab_t *t, void (*fn_ptr)(const char *, unsigned int)){

	//handle error state
	if (t == NULL || fn_ptr == NULL)
		fatal_error("Wrong parameters: htab_foreach");		

	htab_listitem *item;
	//iterates through hash table
	for (unsigned int i = 0; i < t->n; i++){
		if (*(t->htab_size + i) != NULL){
			for (item = *(t->htab_size + i); item != NULL; 
					item = item->next){
				//calls function to print out item
				fn_ptr(item->key, item->data);
			}	
		}
	}
}

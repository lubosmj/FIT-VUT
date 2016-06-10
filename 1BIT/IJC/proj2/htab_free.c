// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9

#include "htable.h"
#include <stdlib.h>

//frees all allocated space and deletes hash table
void htab_free(htab_t *t){

	//handle error state
	if (t == NULL)
		fatal_error("Wrong parameter: htab_free");

	//calls function to clear content of hash table
	htab_clear(t); 

	//free all remaining allocs
	free(t->htab_size);
	free(t);
}

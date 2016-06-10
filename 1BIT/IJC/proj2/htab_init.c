// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9

#include <stdlib.h>
#include "htable.h"

//initialize hash table
htab_t *htab_init(unsigned int size){

	//returns NULL, if the size is 0 or less
	if (size < 1){
		warning_msg("Invalid paramter: htab_init");
		return NULL;
	}
	//allocates memory for hash table
	htab_t *h = calloc(1, sizeof(htab_t));
	h->htab_size = calloc(size, sizeof(htab_listitem));
	//resturns NULL, if memory allocation failed
	if (h == NULL || h->htab_size == NULL)
		return NULL;

	h->hash_fun_ptr = hash_function;
	h->n = size;
	return h;
}

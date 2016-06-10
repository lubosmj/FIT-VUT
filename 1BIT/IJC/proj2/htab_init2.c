// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9

#include <stdlib.h>
#include "htable.h"

//initialize hash table with non-defualt hash function
htab_t *htab_init2(unsigned int size, hash_fn_ptr fn){

	//returns NULL, if the size is 0 or less, or fn points to NULL
	if (size < 1 || fn == NULL){
		warning_msg("Invalid parameter: htab_init2");
		return NULL;
	}
	//allocates memory for hash table
	htab_t *h = calloc(1, sizeof(htab_t));
	h->htab_size = calloc(size, sizeof(htab_listitem));
	//resturns NULL, if memory allocation failed
	if (h == NULL || h->htab_size == NULL)
		return NULL;

	h->hash_fun_ptr = fn;
	h->n = size;
	return h;
}

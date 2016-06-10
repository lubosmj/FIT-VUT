// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9

#include "htable.h"

//default hash function
unsigned int hash_function(const char *str, unsigned int htab_size){

	unsigned int h = 0;
	const unsigned char *p;
	for(p=(const unsigned char*)str; *p != '\0'; p++)
		h = 65599 * h + *p;
	return h % htab_size;
}	

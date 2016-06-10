// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9

#include <stdio.h>
#include "htable.h"

//prints out item with specific key
void print_item(const char *key, unsigned int data){

	//prints out name of item and number of occurrences
	printf("%s\t%d\n", key, data);	
}

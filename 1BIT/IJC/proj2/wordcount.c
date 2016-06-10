// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9

#include <stdio.h>
#include <stdlib.h>
#include "htable.h"
#include "io.h"

int main (void) {

	//initialize hash table
	htab_t *tab = htab_init(12289);

	/*
	 * Size of hash table is usually determined by type of the input. 
	 * Therefore, I chose the size of hash table to be not too big 
	 * or too small. Also, using prime number as a size of hash table 
	 * minimizes clustering and collisions in the hash table. By using
	 * the prime numbers as the size of hash table, there is a good 
	 * chance of obtaining a unique value for hash function.
	 */

	//if an error occured, print error message
	if (tab == NULL){
		fatal_error("An error occured while initializing a hash table!");
		return -1;
	}

	//variable to store loaded word
	char *word = malloc(256);
	//iterates through the file stream until end of file is reached
	//and stores loaded words in hash table
	while (get_word(word, 127, stdin) != EOF){
		//insert loaded word to hash table
		htab_listitem *item = htab_lookup_add(tab, word);
		//handle error state
		if (item == NULL)
			fatal_error("An error occured while loading items!");
	}
	//free allocated space of char pointer
	free(word);
	
	//prints out content of hash table
	htab_foreach(tab, print_item);

	//clear the hash table
	if (tab != NULL)
		htab_free(tab);
	
	return 0;
}

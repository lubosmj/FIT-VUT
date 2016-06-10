// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9

#include <stdlib.h>
#include <string.h>
#include "htable.h"

//search key through the hash table
htab_listitem *htab_lookup_add(htab_t *t, const char *key){

	//check function parameters
	if (t == NULL || key == NULL){
		warning_msg("Invalid pointer: htab_lookup_add");
		return NULL;
	}
	//get position of key in hash table
	unsigned int i = t->hash_fun_ptr(key, t->n);
	htab_listitem *temp = *(t->htab_size + i);
	//iterate through hash table to find key that already exists
	for (htab_listitem *item = *(t->htab_size + i); item != NULL; 
			item = item->next){
		//if the key exists in hash table
		if (!strcmp(key, item->key)){
			//increses counter of occurence
			item->data++;
			//returns pointer to that item
			return item;
		}
	}

	//if the key does not exist, adds a new item to the hash table 
	//and returns pointer to that particular item
	
	//allocate memory for new item
	htab_listitem *newitem = malloc(sizeof(htab_listitem));	
	//allocate memory for key
	newitem->key = malloc((strlen(key) + 1) * sizeof(char));
	//handle errors while allocating memory
	if (newitem == NULL || newitem->key == NULL)
		return NULL;

	//add new item to the hash table
	strcpy(newitem->key, key);
	newitem->data = 1;
	newitem->next = temp;

	*(t->htab_size + i) = newitem;
	//returns pointer to new item
	return newitem;
}

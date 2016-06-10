// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9

#ifndef HTABLE_H
#define HTABLE_H
//function pointer to default hash function
typedef unsigned int (*hash_fn_ptr)(const char *, unsigned int);

//defines item in hash table
typedef struct htab_listitem {
	char *key;
	unsigned int data;
	struct htab_listitem *next;
} htab_listitem;

//defines core of hash table
typedef struct htab_t {
	unsigned int n;	
	hash_fn_ptr hash_fun_ptr;
	htab_listitem **htab_size;
} htab_t;

void warning_msg(const char *fmt, ...);

void fatal_error(const char *fmt, ...);

unsigned int hash_function(const char *str, unsigned int htab_size);

htab_t *htab_init(unsigned int size);

htab_t *htab_init2(unsigned int size, hash_fn_ptr fn);

htab_listitem *htab_lookup_add(htab_t *t, const char *key);

void print_item(const char *key, unsigned int data);

void htab_foreach(htab_t *t, void (*fn_ptr)(const char *, unsigned int));

void htab_remove(htab_t *t, const char *key);

void htab_clear(htab_t *tab);

void htab_free(htab_t *t);
#endif 

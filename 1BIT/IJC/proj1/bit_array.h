// Autor: Lubos Mjachky
// Datum: 25/3/2016
// FIT VUT 2015/2016
// IJC DU1
// prelozeno: gcc 4.9

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "error.h"

typedef unsigned long bit_array_t[];

#define BIT_ULONG (sizeof(unsigned long)*CHAR_BIT)

#define ba_create(jmeno_pole,velikost) unsigned long \
	jmeno_pole[(velikost+(BIT_ULONG-1))/BIT_ULONG + 1] = {velikost,0,}

#define DU1_GET_BIT_(p,i) (p[i/BIT_ULONG + 1]&(1LU<<(i%BIT_ULONG)))?1LU:0LU

#define DU1_SET_BIT_(p,i,b) (b == 1)?(p[i/BIT_ULONG + 1] |= (1LU<<(i%BIT_ULONG))): \
								(p[i/BIT_ULONG] &= ~(1LU<<(i%BIT_ULONG))) 


//inline unsigned long ba_get_bit(unsigned long *a, unsigned int i);
//inline void ba_set_bit(unsigned long *a, unsigned int i, unsigned int v);

#ifdef USE_INLINE

static inline unsigned long ba_size(bit_array_t jmeno_pole);
static inline unsigned long ba_get_bit(bit_array_t jmeno_pole, unsigned int index);
static inline void ba_set_bit(bit_array_t jmeno_pole, unsigned int index,
		unsigned int vyraz);


static inline unsigned long ba_size(bit_array_t jmeno_pole){

	return jmeno_pole[0];

}

static inline unsigned long ba_get_bit(bit_array_t jmeno_pole, unsigned int index){

	if (index > ba_size(jmeno_pole))
		fatal_error("Index %ld mimo rozsah 0..%ld", (long)index, 
				(long)ba_size(jmeno_pole));

	return DU1_GET_BIT_(jmeno_pole,index);

}

static inline void ba_set_bit(bit_array_t jmeno_pole, unsigned int index, 
		unsigned int vyraz){

	if (index > ba_size(jmeno_pole))
		fatal_error("Index %ld mimo rozsah 0..%ld", (long)index, 
				(long)ba_size(jmeno_pole));

	DU1_SET_BIT_(jmeno_pole,index,vyraz);

}

#else

#define ba_size(jmeno_pole) (jmeno_pole[0])  

#define ba_get_bit(jmeno_pole,index) ((index > ba_size(jmeno_pole))?  \
		(fatal_error("Index %ld mimo rozsah 0..%ld", (long)index, \
					 (long)ba_size(jmeno_pole)),0):				\
		DU1_GET_BIT_(jmeno_pole, index))

#define ba_set_bit(jmeno_pole, index, vyraz) ((index > ba_size(jmeno_pole))?  \
		(fatal_error("Index %ld mimo rozsah 0..%ld", (long)index, \
					 (long)ba_size(jmeno_pole)),0):				\
		DU1_SET_BIT_(jmeno_pole, index, vyraz))

#endif

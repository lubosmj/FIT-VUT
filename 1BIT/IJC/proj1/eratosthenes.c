// Autor: Lubos Mjachky
// Datum: 25/3/2016
// FIT VUT 2015/2016
// IJC DU1
// prelozeno: gcc 4.9

#include <stdio.h>
#include <math.h>
#include "eratosthenes.h"

void Eratosthenes(bit_array_t pole){

	unsigned int s;

	unsigned int size = ba_size(pole);
	unsigned int sqr = sqrt(size);

	DU1_SET_BIT_(pole, 0, 1);
	DU1_SET_BIT_(pole, 1, 1);

	for (s = 4; s <= size; s+=2)
		DU1_SET_BIT_(pole, s, 1);

	for (unsigned int i = 3; i < sqr; i+=2){
		if (ba_get_bit(pole, i) == 0){
			for (s = i*i; s <= size; s+=i){
				printf("%lu\n", ba_get_bit(pole, s));
				DU1_SET_BIT_(pole, s, 1);
			}
		}
	}

}

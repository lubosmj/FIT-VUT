// Autor: Lubos Mjachky
// Datum: 25/3/2016
// FIT VUT 2015/2016
// IJC DU1
// prelozeno: gcc 4.9

#include <stdio.h>
#include "eratosthenes.h"

#define PRIMES 10
#define N 202000000

int main(void) {

	unsigned int primes_array[PRIMES];

	ba_create(bit_array, N);
	Eratosthenes(bit_array);

	//saves to the array last 10 prime numbers up to N
	for (unsigned int j = 1, i = N; j <= 10; i--) {
		if (ba_get_bit(bit_array, i) == 0){
			primes_array[j] = i;
			j++;
		}
	}

	//prints 10 last prime number from lowest to highest prime number
	for (int i = PRIMES; i >= 1; i--) {
		printf("%d\n", primes_array[i]);
	}

	return 0;

}

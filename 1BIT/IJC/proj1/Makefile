# Autor: Lubos Mjachky
# Datum: 25/3/2016
# FIT VUT 2015/2016
# IJC DU1
# prelozeno: gcc 4.9

CC=gcc
CFLAGS=-std=c99 -O2 -Wall -Wextra -pedantic
LDFLAGS=-lm

all: primes primes-inline steg-decode

.PHONY: clean

primes-inline: primes.o error.o eratosthenes.o
	$(CC) $(CFLAGS) primes-inl.o error.o eratosthenes-inl.o -o primes-inline $(LDFLAGS)


primes: primes.o error.o eratosthenes.o 
	$(CC) $(CFLAGS) primes.o error.o eratosthenes.o -o primes  $(LDFLAGS)
primes.o: primes.c eratosthenes.h bit_array.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c primes.c -o primes-inl.o $(LDFLAGS)
	$(CC) $(CFLAGS) -c primes.c -o primes.o $(LDFLAGS)
eratosthenes.o: eratosthenes.c eratosthenes.h bit_array.h 
	$(CC) $(CFLAGS) -DUSE_INLINE -c eratosthenes.c -o eratosthenes-inl.o $(LDFLAGS)
	$(CC) $(CFLAGS) -c eratosthenes.c -o eratosthenes.o $(LDFLAGS)
	
steg-decode: ppm.o steg-decode.o eratosthenes.o
	$(CC) $(CFLAGS) ppm.o steg-decode.o eratosthenes.o -o steg-decode $(LDFLAGS)
steg-decode.o: steg-decode.c ppm.h eratosthenes.h 
	$(CC) $(CFLAGS) -c steg-decode.c -o steg-decode.o $(LDFLAGS)
ppm.o: ppm.c ppm.h 
	$(CC) $(CFLAGS) -c ppm.c -o ppm.o $(LDFLAGS)

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o error.o

clean:
	-rm  -f *.o 

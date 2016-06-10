# Autor: Lubos Mjachky
# Datum: 25/4/2016
# FIT VUT 2015/2016
# IJC DU2
# prelozeno: gcc 4.9

#c compiler
CC=gcc
#c++ compiler
CPPC=g++

#flags to compile with
CFLAGS= -std=c99 -Wall -Wextra -pedantic 
CPPFLAGS= -std=c++11 -Wall -Wextra -pedantic 

#object files generated for hash table
HTAB_OBJ= htab_init.o htab_lookup_add.o htab_foreach.o htab_clear.o htab_free.o htab_remove.o htab_init2.o hash_function.o htab_error.o htab_print_item.o

#set fake target
.PHONY: clear

#files to be compiled
all: tail tail2 wordcount wordcount-dynamic

#generate tail programs
tail: tail.o
	$(CC) $(CFLAGS) $< -o $@
tail2: tail2.o
	$(CPPC) $(CPPFLAGS) $< -o $@

#generate tail object files
tail.o: tail.c
	$(CC) $(CFLAGS) -c $< -o $@
tail2.o: tail2.cc
	$(CPPC) $(CPPFLAGS) -c $< -o $@

#generate static library
libhtable.a: $(HTAB_OBJ)
	ar crs $@ $(HTAB_OBJ)
#generate shared library
libhtable.so: $(HTAB_OBJ)
	$(CC) $(CFLAGS) -shared -fPIC -o $@ $(HTAB_OBJ)

#executable file running with static library
wordcount: wordcount.o io.o libhtable.a
	$(CC) $(CFLAGS) -static wordcount.o io.o -L. -lhtable -o $@
#executable file running with shared library
wordcount-dynamic: wordcount.o io.o libhtable.so
	$(CC) $(CFLAGS) -dynamic wordcount.o io.o -L. -lhtable -o $@

#universal rule to generate all object files necessary for hash table
%.o : %.c
	$(CC) $(CFLAGS) -c -fPIC $< -o $@

## ## ## 
#rules without the body; completed by universal rule
htab_lookup_add: htab_lookup_add.c htable.h
htab_print_item.o: htab_print_item.c htable.h
htab_foreach.o: htab_foreach.c htable.h
htab_clear.o: htab_clear.c htable.h
htab_free.o: htab_free.c htable.h
htab_remove.o: htab_remove.c htable.h
htab_error.o: htab_error.c htable.h
htab_init.o: htab_init.c htable.h
htab_init2.o: htab_init.c htable.h

hash_function.o: hash_function.c htable.h
## ## ## 

#generate io object file
io.o: io.c io.h
	$(CC) $(CFLAGS) -c $< -o $@


#remove all generated files
clear:
	-rm -f *.o *.a *.so tail tail2 wordcount wordcount-dynamic

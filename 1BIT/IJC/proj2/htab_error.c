// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "htable.h"

//prints out warning message entered as a parameter
void warning_msg(const char *fmt, ...) {

	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "ERROR: ");
	//prints to stderr error message
	vfprintf(stderr, fmt, args); 
	fprintf(stderr, "\n"); 
	va_end(args);
}

//prints out error message entered as a parameter and exits program
void fatal_error(const char *fmt, ...) {

	va_list args;
	va_start(args, fmt); 
	fprintf(stderr, "ERROR: "); 
	//prints to stderr error message
	vfprintf(stderr, fmt, args); 
	fprintf(stderr, "\n");
	va_end(args);  
	exit(1);  
}

// Autor: Lubos Mjachky
// Datum: 25/3/2016
// FIT VUT 2015/2016
// IJC DU1
// prelozeno: gcc 4.9
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "error.h"

void warning_msg(const char *fmt, ...) {

	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "CHYBA: ");		
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
}

void fatal_error(const char *fmt, ...) {

	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "CHYBA: ");		
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
	exit(1);
}

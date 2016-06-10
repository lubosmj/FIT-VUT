// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9

#include <stdio.h>
#include <string.h>
#include "io.h"

//function to read word 
int get_word (char *s, int max, FILE *f) {

	//static integer to prevent printing warning 
	//message to stdout more than once at runtime
	static unsigned int status = 0;
	//length of word
	int length = 0;
	//auxiliary array to store loaded word
	char g[max + 1];

	//loads word, if end of the file was reached, returns EOF
	if (fscanf(f, "%129s", s) == EOF)
		return EOF;
	//detect if maximum length of word was reached
	if ((int)strlen(s) > max){
		int ret;
		//skip whole line
		while(((ret = fgetc(f)) != EOF) && (ret != '\n'));
		//print warning message only once at runtime
		if (status == 0){
			//set status to 1 to exclude printing warning message
			status = 1;
			//prints out warning message to stderr
			fprintf(stderr, "Max length of word was reached," 
					"truncating...\n");
		}
	}
	s[max] = '\0';

	//copy word from array s to array g until max length is reached
	for (int i = 0; i < max; i++){
		g[i] = s[i];
	}
	//add null terminating character to flag end of string
	g[max] = '\0';
	//get length of word stored in array 
	length = strlen(g);

	//return length of word
	return length;
}

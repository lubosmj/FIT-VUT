// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
// prelozeno: gcc 4.9
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//max size of buffer
#define BUFFERSIZE 1000000

//max length of line
#define MAXLINE 510

//print out lines from file stream
void print_lines(FILE *file){

	int character;			//stores ASCII value of character
	int chnum = 0;			//number of characters in one line

	static int flag = 0;	//stores status of error message;
	//error message should be printed only once

	//read lines from file until end of file
	while ((character = fgetc(file)) != EOF){
		//disable buffering on stdout
		setbuf(stdout, NULL);
		//check length of line
		if (chnum++ > MAXLINE){
			//if line is too long, skip all characters until end of line
			while ((character = fgetc(file)) != '\n');
			//print error message only once at runtime
			if (flag == 0){
				flag = 1;
				fprintf(stderr,"\nError: Maximum length of line was reached");
			}
			chnum = 0;
		}
		//if end of line occurred, set counter of characters to zero 
		if (character == '\n')
			chnum = 0;

		//print out character
		printf("%c", character);
	}
}

//count number of lines from file stream
int count_lines(FILE *file){

	int lines = 0, temp = 0;

	while (!feof(file)){
		if ((temp = fgetc(file)) == '\n')
			lines++;
	}
	return lines;
}

//print last N lines from given stream
int tail(int status, int num, FILE *file){

	int file_lines = 0;		//number of lines in file	
	int line = 0;			//number of last line to be skipped 
	int file_chars = 0;		//number of characters
	int temp = 0;			//temporarily sotres one loaded character

	static int flag = 0;	//stores status of error message;
	//error message should be printed only once

	//reading from file stream
	if (status == 1){		

		file_lines = count_lines(file);
		fseek (file, 0, SEEK_SET);

		//if number of lines is less than number of lines to be printed
		//print everything and exit
		if (file_lines < num+1){
			print_lines(file);
			return 0;
		}

		//get number of line, which will be skipped as a last line
		line = file_lines - num;

		//reset counter of lines
		file_lines = 0;

		//count number of characters which will be skipped in fseek function
		while (!feof(file)){
			if ((temp = fgetc(file)) == '\n')
				file_lines++;
			if (line == file_lines)
				break;
			file_chars++;
		}

		//"add" \n character
		file_chars++;
		//set position of stream to the given offset (file_chars)
		fseek (file, file_chars, SEEK_SET);
		//print last N lines from file
		print_lines(file);
	}

	//reading from stdin
	else if (status == 2){

		int *text;	//integer array, stores ASCII values of characters

		//allocate default buffer size
		text = malloc(sizeof(int) * BUFFERSIZE);
		//handle error in allocation
		if (text == NULL) {
			fprintf(stderr, "Error while allocating memory\n");
			return -1;
		}

		int memall = BUFFERSIZE;	//memory to be allocated 
		int read = 1;				//number of all loaded characters
		int chnum = 0;				//number of characters in one line
		int ch = getchar();			//stores ASCII value of character
		
		int *tmp_pointer;			//points to allocated memory before reallocation process
		//read from stdin and load all lines to buffer
		while (ch != EOF) {
			//check if allocated memory is big enough to store all lines
			if (read >= memall) {
				//if length of line is more than allowed
				//skip all characters until end of line or EOF
				if (read > MAXLINE+1){
					while((ch = getchar()) != '\n' || (ch = getchar()) != EOF);	
				}	
				//resize buffer to prevent buffer overflow
				else {
					//enlarge buffer size
					memall = memall * 2;
					//store pointer to allocated memory
					tmp_pointer = text;
					//alocate new space for buffer
					text = realloc(text, memall * sizeof(int));
					//check if reallocation was completed successfully
					if (text == NULL) {
						fprintf(stderr, "Error while allocating memory\n");
						//free allocated space
						free(tmp_pointer);
						return -1;
					}
				}
			}
			//store loaded character into buffered array of characters
			text[read-1] = ch;
			read++;
			ch = getchar();
		}

		int lines = 0;				//number of lines
		int print = read;			//index of array, stores position
		//of characters to be printed on stdout

		//skip lines which will be not printed 
		while (lines <= num && print > 0) {
			print--;
			if (print == 0)
				break;
			if (text[print-1] == '\n') 
				lines++;
		}

		//print out character by character to stdout
		while (print < read - 1) {
			//disable buffering
			setbuf(stdout, NULL);
			//check max length of line
			if (chnum++ > MAXLINE){
				//skip all characters until end of line
				while (text[print] != '\n')
					print++;

				//print error message if max length of line was exceeded
				if (flag == 0){
					//print error message only once at runtime
					//flag is static variable, so it will store its value
					flag = 1;
					fprintf(stderr,"\nError: Maximum length of line"
							" was reached");
				}
				//reset counter of characters in line
				chnum = 0;
			}
			//set number of characters in line to zero if end of line occurred
			if (text[print] == '\n')
				chnum = 0;
			//print out character to stdout
			printf("%c", text[print]);
			//increase index of array
			print++;
		}
		//free all allocated space 
		free(text);
	}
	return 0;
}

int main (int argc, char **argv){

	FILE *fp = NULL;	//file pointer
	char *temp;			//temporary variable used in "strtol"
	int lines;			//number of lines to be printed
	int err = 0;		//stores type of error 

	if ((argc > 2) && (argc < 5)){
		//tail '-n' num file
		if (!strcmp(*(argv+1), "-n")){
			lines = strtol(*(argv+2), &temp, 10);
			if (*temp == '\0'){
				if 	(argc == 4){
					if ((fp = fopen(*(argv+3), "r")) != NULL){
						if (lines < 0)
							lines = -lines;
						tail (1, lines, fp);
					}
					else {
						err = 3;
						goto handle_error_args;
					}
				}
				//read from stdin
				else if (argc == 3){
					if (lines < 0)
						lines = -lines;
					tail(2, lines, fp);
				}
			}
			//wrong number of lines
			else {
				err = 2;
				goto handle_error_args;
			}
		}
		//tail file '-n' num
		else if (!strcmp(*(argv+2), "-n")){
			if (argc == 4){
				lines = strtol(*(argv+3), &temp, 10);
			}
			else {
				err = 1;
				goto handle_error_args;
			}
			if (*temp == '\0'){ 
				if 	(argc == 4){
					if ((fp = fopen(*(argv+1), "r")) != NULL){
						tail(1, lines, fp);
					}
					else {
						err = 3;
						goto handle_error_args;
					}
				}
			}
			//wrong number of lines
			else {
				err = 2;
				goto handle_error_args;
			}
		}

		//tail -num file
		else if (argc == 3){
			if (*(*(argv+1)) == '-'){
				lines = strtol(*(argv+1), &temp, 10);
				if (*temp == '\0'){
					if ((fp = fopen(*(argv+2), "r")) != NULL){
						if (lines < 0)
							lines = -lines; //workaround 
						tail(1, lines, fp);
					}
					else {
						err = 3;
						goto handle_error_args;
					}
				}
				else {
					err = 2;
					goto handle_error_args;
				}
			}
			else {
				err = 1;
				goto handle_error_args;
			}
		}
		else {
			err = 1;
			goto handle_error_args;
		}
	}
	else if (argc == 2) {
		//tail - 
		if (!strcmp(*(argv+1), "-")){
			tail(2, 10, fp);
		}
		//tail -num
		else if ((*(*(argv+1)) == '-') && (strcmp(*(argv+1), "-"))){
			lines = strtol(*(argv+1), &temp, 10);
			lines = -lines;
			if (*temp == '\0'){
				tail(2, lines, fp);
			}
			else if (!strcmp(*(argv+1), "-")){
				tail(2, 10, fp);
			}
			else {
				err = 2;
				goto handle_error_args;
			}
		}
		//tail file
		else if ((fp = fopen(*(argv+1), "r")) != NULL){
			tail(1, 10, fp);
		}
		else {
			err = 1;
			goto handle_error_args;
		}
	}
	//read from stdin
	else if (argc == 1){
		tail(2, 10, fp);
	}
	else {
		err = 1;
		goto handle_error_args;
	}
	if (fp != NULL)
		fclose(fp);
	return EXIT_SUCCESS;


handle_error_args:

	fprintf(stderr, "Error: ");
	if (err == 0){
		fprintf(stderr, "An unexpected error!\n");
	}
	else if (err == 1){
		fprintf(stderr, "Wrong arguments entered!\n");
	}
	else if (err == 2){
		fprintf(stderr, "Wrong number of lines entered!\n");
	}
	else if (err == 3){
		fprintf(stderr, "Cannot open file!\n");
	}
	return EXIT_FAILURE;
}

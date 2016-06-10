// Autor: Lubos Mjachky
// Datum: 25/4/2016
// FIT VUT 2015/2016
// IJC DU2
#include <cstring>
#include <iostream>
#include <istream>
#include <fstream>
#include <queue>
#include <string>
#include <iterator>

using namespace std; 

int tail (unsigned int lines_to_print, istream& f){

	//disable synchronization with stdio C library
	std::ios_base::sync_with_stdio(false);
	
	unsigned int lines = 0;				//number of lines
	unsigned int finlin = 0;			//number of first line to be printed
	string line;						//stores line in string format

	queue <string> array;				//STL container to store all lines

	//counts number of lines and stores whole lines into container
	while (getline(f, line)){
		//add line to container
		array.push(line);
		//increment number of lines
		lines++;
	}
	
	//check if number of lines is less than lines to be printed
	if (lines_to_print > lines){
		finlin = 0;
		lines_to_print = lines;
	}
	else 
		finlin = lines - lines_to_print;
	
	//skip and pop all lines which will be not printed out
	for (lines = 0; ++lines <= finlin; array.pop());
	//print out last N lines and pop them out from container
	for (lines = 0; ++lines <= lines_to_print; array.pop())
		cout << array.front() << endl;

	return 0;
}

int main (int argc, char **argv){

	ifstream fp;		//file pointer
	char *temp;			//temporary variable used in "strtol"
	int lines;			//number of lines to be printed
	int err = 0;		//stores type of error 

	//check number of entered arguments
	if ((argc > 2) && (argc < 5)){
		//tail '-n' num file
		if (!strcmp(*(argv+1), "-n")){
			lines = strtol(*(argv+2), &temp, 10);
			if (*temp == '\0'){
				if 	(argc == 4){
					//open file
					fp.open(*(argv+3));
					//check if file was opened successfully
					if (fp.is_open()){
						if (lines < 0)
							lines = -lines;
						tail (lines, fp);
					}
					else {
						//handle error
						err = 3;
						goto handle_error_args;
					}
				}
				//read from stdin
				else if (argc == 3){
					if (lines < 0)
						lines = -lines;
					tail(lines, cin);
				}
			}
			//wrong number of lines
			else {
				//handle error
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
				//handle error
				err = 1;
				goto handle_error_args;
			}
			if (*temp == '\0'){ 
				if 	(argc == 4){
					fp.open(*(argv+1));
					if (fp.is_open()){					
						tail(lines, fp);
					}
					else {
						//handle error
						err = 3;
						goto handle_error_args;
					}
				}
			}
			//wring number of lines
			else {
				//handle error
				err = 2;
				goto handle_error_args;
			}
		}

		//tail -num file
		else if (argc == 3){
			if (*(*(argv+1)) == '-'){
				lines = strtol(*(argv+1), &temp, 10);
				if (*temp == '\0'){
					//open file
					fp.open(*(argv+2));
					//check if file was opene successfully
					if (fp.is_open()){
						if (lines < 0)
							lines = -lines; 
						tail(lines, fp);
					}
					else {
						//handle error
						err = 3;
						goto handle_error_args;
					}
				}
				else {
					//handle error
					err = 2;
					goto handle_error_args;
				}
			}
			else {
				//handle error
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
			tail(10, cin);
		}
		ifstream fp(*(argv+1));
		//tail -num
		if (*(*(argv+1)) == '-'){
			lines = strtol(*(argv+1), &temp, 10);
			if (lines < 0)
				lines = -lines;
			if (*temp == '\0'){
				tail(lines, cin);
			}
			else if (!strcmp(*(argv+1), "-")){
				tail(10, cin);
			}
			else {
				//handle error
				err = 2;
				goto handle_error_args;
			}
		}
		//tail file
		else if (fp.good()){
			//check if file was opened successfully
			if (fp.is_open()){					
				tail(10, fp);
			}
			else {
				//handle error
				err = 3;
				goto handle_error_args;
			}
		}
		else {
			//handle error
			err = 1;
			goto handle_error_args;
		}
	}
	//read from stdin
	else if (argc == 1){
		tail(10, cin);
	}
	else {
		//handle error
		err = 1;
		goto handle_error_args;
	}

	//close opened file
	if (fp.is_open())
		fp.close();

	return EXIT_SUCCESS;

//handle errors 
handle_error_args:

	cerr << "Error: ";
	if (err == 0){
		cerr << "An unexpected error!" << endl;
	}
	else if (err == 1){
		cerr << "Wrong arguments entered!" << endl;
	}
	else if (err == 2){
		cerr << "Wrong number of lines entered!" << endl;
	}
	else if (err == 3){
		cerr << "Cannot open file!" << endl;
	}
	return EXIT_FAILURE;
}

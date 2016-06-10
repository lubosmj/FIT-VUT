// Autor: Lubos Mjachky
// Datum: 25/3/2016
// FIT VUT 2015/2016
// IJC DU1
// prelozeno: gcc 4.9

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "eratosthenes.h"
#include "ppm.h"
#include "error.c"

int main (int argc, char **argv) {

	char *temp;
	int bit_value, pos = 0, string = 0;
	ba_create(ppm_array, MAX);
	Eratosthenes(ppm_array);
	if (argc > 1 && argc < 3) {
		if ((temp = strrchr(*(argv + 1), '.' )) != NULL){
			if (strcmp(temp + 1, "ppm") == 0){

				struct ppm *image = ppm_read(*(argv + 1));
				if (image == NULL){
					free(image);
					fatal_error("Nepodarilo sa uspesne vytvorit strukturu");
				}

				unsigned long ppm_size = image->xsize*image->ysize*3;
				for (unsigned int i = 2; i < ppm_size-1; i++){
					if (ba_get_bit(ppm_array,i) == 0){

						bit_value = image->data[i];
						bit_value &= 1;

						if (bit_value == 1)
							string |= 1<<pos;
						else if (bit_value == 0)
							string &= ~(1<<pos);

						if (pos < CHAR_BIT-1){
							pos++;
						}
						else if (isprint(string)){
							printf("%c",string);
							pos = 0;
							string = 0;
						}
						else if (string == '\0'){
							break;
						}
						else {
							free(image);
							fatal_error("Pri vypisovani znakov nastala neznama chyba");
						}
					}	
				}

				printf("\n");
				free(image);
			}
			else {
				warning_msg("Subor nie je formatu PPM");
				return -1;
			}
		}
		else {
			warning_msg("Bol zadany nespravny argument");
			return -1;
		}
	}	
	else {
		warning_msg("Nebol zadany spravny pocet argumentov");
		return -1;
	}
	return 0;
}

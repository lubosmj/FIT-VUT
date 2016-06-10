// Autor: Lubos Mjachky
// Datum: 25/3/2016
// FIT VUT 2015/2016
// IJC DU1
// prelozeno: gcc 4.9

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ppm.h"
#include "error.h"

struct ppm *ppm_read(const char *filename){

	FILE *fp = fopen (filename, "rb");
	if (fp == NULL){
		warning_msg("Nepodarilo sa otvorit subor");
		return NULL;
	}

	int x_i, y_i;
	char x[6],y[6], magic_num[3];
	char *temp1, *temp2;

	int scan_num = fscanf(fp, "%2s %5s %5s 255\n", magic_num, x, y);

	if (scan_num != 3){
		fclose(fp);
		warning_msg("Nepodarilo sa precitat hlavicku suboru");
		return NULL;
	}

	int cmp_res = strcmp(magic_num, "P6");
	x_i = strtol(x, &temp1, 10);
	y_i = strtol(y, &temp2, 10);
	unsigned long ppm_size = x_i*y_i*3;
	if (ppm_size > MAX){
		fclose(fp);
		warning_msg("Velkost obrazovych dat prekrocila povoleny limit");
		return NULL;
	}

	if (*temp1 == '\0' && *temp2 == '\0' && cmp_res == 0 
			&& x_i > 0 && y_i > 0){
		struct ppm *ppm1 = malloc(ppm_size + sizeof(struct ppm));
		if (ppm1 == NULL){
			fclose(fp);
			warning_msg("Nepodarilo sa naalokovat dostatocnu velkost pamate");
			return NULL;
		}
		ppm1->xsize = x_i;
		ppm1->ysize = y_i;

		unsigned long fr_ret = fread(ppm1->data, 1, ppm_size, fp);
		if (fr_ret != ppm_size){
			free(ppm1);
			fclose(fp);
			warning_msg("Velkost nacitanych dat nezodpoveda hlavicke subora");
			return NULL;
		}

		fclose(fp);
		return ppm1;
	}
	else {
		fclose(fp);
		warning_msg("Subor nevyhovuje podmienkam formatu PPM");
		return NULL;
	}

}


int ppm_write(struct ppm *p, const char *filename){

	FILE *fp = fopen(filename, "wb");
	if (fp == NULL){ 
		warning_msg("Doslo k chybe pri otvarani suboru");
		return -1;
	}

	int fprint_ret = fprintf(fp, "P6 %u %u 255 ", p->xsize, p->ysize);
	if (fprint_ret < 0){
		warning_msg("Doslo k chybe pri zapisovani do suboru");
		fclose(fp);
		return -1;
	}

	unsigned long size = p->xsize*p->ysize*3;
	unsigned long fwrite_ret = fwrite(p->data, 1, size, fp);			
	if (size != fwrite_ret){
		warning_msg("Doslo k chybe pri zapise binarnych dat");
		fclose(fp);
		return -1;
	}	
	return 0;

}

// Autor: Lubos Mjachky
// Datum: 25/3/2016
// FIT VUT 2015/2016
// IJC DU1
// prelozeno: gcc 4.9

#define MAX 5000*5000*3

struct ppm {
	unsigned xsize;
	unsigned ysize;
	char data[];
};


struct ppm *ppm_read(const char *filename);
int ppm_write(struct ppm *p, const char *filename);

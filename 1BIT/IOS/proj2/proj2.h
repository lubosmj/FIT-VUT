//macro to print error message and exit program with exit code 1
#define error(s,n)  do {\
	if (n == 1){\
		fprintf(stderr, "ARG-ERROR: " "%s!\n", s);\
		exit(1);\
	}\
	else {\
		fprintf(stderr, "SYSTEM-CALL: " "%s!\n", s);\
		exit(2);\
	}\
} while (0);

/** structure of parameters */
typedef struct params{
	/** number of passengers */
	int P;
	/** car capacity */
	int C;
	/** max time of process creation */
	int PT;
	/** max time of ride */
	int RT;
} params_t;


long rand_int(long max);

void force_exit();     

void car(params_t *param, FILE *fp);     

void passenger(params_t *param, FILE *fp);

void free_resources(void);      

void load_parameters(int argc, char *argv[], params_t *lok_param);

void load_resources(params_t *param);      

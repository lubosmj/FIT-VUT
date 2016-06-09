/*
 * This program was developed by Lubos Mjachky in 2015.
 *
 * It is able to calculate logarithm of any number entered by user
 * as an argument. In this program, there are used two methods of getting 
 * the logarithmic number. There is an algorithm to compute logarithm by Taylor`s 
 * polynomial and algorithm for computing logarithm by continued fractions. Also,
 * this program is able to detect number of iterations which are needed to compute
 * logarithmic number with a certain accuracy.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

/*This function returns absolute value of a number*/
double abs_value(double x){
	if (x < 0){
		return -x;
	}
	else {
		return x;
	}
}

/*Function to calculate logarithm using the Taylor`s polynomial*/
double taylor_log(double x, unsigned n){

	if ((x < 0.0) || (x != x)){
		return NAN;
	}
	if (x == 0.0){
		return -INFINITY;
	}
	if (x == INFINITY){
		return INFINITY;
	}
	double getXt = 0.0, getPow = 1.0;
	unsigned i;
	if (x > 1){
		for (i = 1; i <= n; i++){
			getPow *= ((x - 1) / x);
			getXt += ((getPow) / i);
		}
	}
	if (x <= 1){
		x = 1 - x;
		for (i = 1; i <= n; i++){
			getPow *= x;
			getXt -= (getPow / i);		
		}
	}
	return getXt;
}

/*Function to compute logarithm by continued fractions*/
double cfrac_log(double x, unsigned n){

	if ((x < 0.0) || (x != x)){
		return NAN;
	}
	if (x==0.0){
		return -INFINITY;
	}
	if (x==INFINITY){
		return INFINITY;
	}
	double getXc, xMul, xOld = 0.0;
	double nd = (double)n;
	x = (x - 1) / (x + 1);
	for (nd = nd - 1; nd > 0; nd--){
		xMul = 2.0 * nd + 1;
		getXc = ((nd * nd) * (x * x)) / (xMul - xOld);
		xOld = getXc;
	}
	return 2 * x / (1 - getXc);
}

/*Function to compute required number of iterations via Taylor`s polynomial*/
unsigned int get_taylor_log_iter(double x, double eps){

	double xTaylor;
	double xMathL = log(x);
	unsigned int i = 1;
	eps = abs_value(eps);
	if (eps != eps){
		return i;
	}
	for(i = 1 ;; i++){
		xTaylor = taylor_log(x,i);
		if ((abs_value(xTaylor-xMathL) <= eps) || (xTaylor != xTaylor) || (xTaylor == -INFINITY) 
				|| (xTaylor == INFINITY)){
			break;
		}
	}
	return i;
}

/*Function to compute required number of iterations via continued fractions*/
unsigned int get_frac_log_iter(double x, double eps){

	double xFrac;
	double xMathL = log(x);
	unsigned int i = 1;
	eps = abs_value(eps);
	if (eps != eps){
		return i;
	}
	for (i = 1 ;; i++){
		xFrac = cfrac_log(x,i);
		if ((abs_value(xFrac-xMathL) <= eps) || (xFrac != xFrac) || (xFrac == -INFINITY)
				|| (xFrac == INFINITY)){
			break;
		}
	}
	return i;
}

int main(int argc, char *argv[]){
	if (argc > 3){
		if (!strcmp(argv[1], "--log") && argc==4){
			char *tempL1, *tempL2;
			double x = strtod(argv[2],&tempL1);
			double xIte = strtol(argv[3],&tempL2,10);
			/*Checks if there are any redundant characters inside the argument*/
			if ((*tempL1 != '\0') || (*tempL2 !='\0')){
				printf("ERROR: There are presented characters instead of numbers!\n");
				return -1;
			}
			/*Checks if there is a valid number of iterations*/
			if ((xIte <= 0) || (xIte > UINT_MAX)){
				printf("ERROR: Entered wrong number of iterations!\n");
				return -1;
			}
			xIte = (unsigned)xIte;
			printf("       log(%g) = %.12g\n", x, log(x));
			printf("    cf_log(%g) = %.12g\n", x, cfrac_log(x,xIte));
			printf("taylor_log(%g) = %.12g\n", x, taylor_log(x,xIte));
		}

		else if (strcmp(argv[1], "--iter")==0 && argc==5){
			char *tempI1, *tempI2, *tempI3;
			unsigned iterNumt, iterNumc;
			double x=strtod(argv[2],&tempI1);
			double y=strtod(argv[3],&tempI2);
			double epsInput=strtod(argv[4],&tempI3);

			unsigned iterNumt1= get_taylor_log_iter(x, epsInput);
			unsigned iterNumt2= get_taylor_log_iter(y, epsInput);

			unsigned iterNumc1= get_frac_log_iter(x, epsInput);
			unsigned iterNumc2= get_frac_log_iter(y, epsInput);
			/*Checks if there are any redundant characters inside the argument*/
			if (*tempI1!='\0' || *tempI2!='\0' || *tempI3!='\0'){
				printf("ERROR: There are presented characters instead of numbers!\n");
				return 0;
			}
			/*Checks if the MIN value of interval is not higher than MAX value*/
			if (x>y){
				printf("ERROR: MIN value cannot be higher than MAX value!\n");
				return 0;
			}
			/*Check required number of iterations for Taylor`s polynomial*/
			if (iterNumt1>iterNumt2){
				iterNumt=iterNumt1;
			}
			else {
				iterNumt=iterNumt2;
			}
			/*Check required number of iterations to compute logarithm via continued fractions*/
			if (iterNumc1>iterNumc2){
				iterNumc=iterNumc1;
			}
			else {
				iterNumc=iterNumc2;
			}

			printf("       log(%g) = %.12g\n", x, log(x));
			printf("       log(%g) = %.12g\n", y, log(y));
			printf("continued fraction iterations = %u\n",iterNumc);
			printf("    cf_log(%g) = %.12g\n",x, cfrac_log(x,iterNumc));
			printf("    cf_log(%g) = %.12g\n",y, cfrac_log(y,iterNumc));
			printf("taylor polynomial iterations = %u\n",iterNumt);
			printf("taylor_log(%g) = %.12g\n",x, taylor_log(x,iterNumt));
			printf("taylor_log(%g) = %.12g\n",y, taylor_log(y,iterNumt));
		}
		else {
			printf("ERROR: Invalid arguments entered!\n");
		}
	}
	else {
		printf("ERROR: Not enough arguments entered!\n");
	}
	return 0;
}

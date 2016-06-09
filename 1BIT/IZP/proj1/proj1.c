/*
 * This program was developed by Lubos Mjachky in 2015.
 * 
 * It has to be able to recognize whether the entered data are words, 
 * numbers or palindromes. If the input text is in format YYYY-MM-DD 
 * and it is meant to be a date, the output will be the day attributable 
 * to that particular date. Also, when the input text is a number, the program 
 * detects if the number is a prime or not. 
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

/*Function to get length of input text*/
int inputLen(const char *c){
	const char *p = c;
	while (*c){
		c++;
	}
	return (c - p);
}

/*Function to convert string to integer*/
unsigned strToInt(char *c){
	unsigned iPlus = 0;
	for (int i = 0; c[i] != '\0'; i++){
		iPlus = iPlus * 10 + (c[i] - '0');
	}
	return iPlus;
}

/*
 * Function to read  numbers in date format YYYY-MM-DD, 
 * this function returns converted integer value and it is called
 * only from isDate() function.
 */
int getDateNumber(char *dn, int g){
	char dateArray[2];
	int j;
	for (j = 0; (dn[g] >= '0') && (dn[g] <= '9'); j++, g++){
		dateArray[j] = dn[g];
	}
	dateArray[j] = 0;
	return strToInt(dateArray);
}

/*Function to check if the input text is a date in format YYYY-MM-DD*/
bool isDate (char *d){
	struct tm itime = {0};
	char *weekday[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

	/*
	 * Checks if there is a dash in the 5th and 8th position of a string.
	 * If yes, then function getDateNumber() is called. 
	 */
	if ((d[4]=='-') && (d[7]=='-')){
		itime.tm_year = getDateNumber(d,0);
		itime.tm_mon = getDateNumber(d,5);
		itime.tm_mday = getDateNumber(d,8);
	}
	/*
	 * To verify date format YYYY-MM-DD, the following if statement
	 * checks if there is in the 10th position of a string a digit. 
	 */
	if ((d[9]>='0') && (d[9]<='9')){
		if ((itime.tm_mon!=0) && (itime.tm_mday != 0) && (d[10] == '\0')){
			if ((itime.tm_year > 1899) && (itime.tm_mon < 13) && (itime.tm_mon > 0) 
					&& (itime.tm_mday < 32) && (itime.tm_mday > 0)){	
				itime.tm_year = itime.tm_year - 1900;
				itime.tm_mon = itime.tm_mon - 1; 
				if (mktime(&itime) != -1){
					printf("date: %s %s\n", weekday[itime.tm_wday],d);
					return true;
				}
			}
		}
	}
	return false;
}

/*Function to check if the input text is a number*/
bool isNumber (char *c){
	int i = 0;
	int tmp = 0;
	while (c[i] != '\0'){
		if ((c[i] == '0') && (c[i + 1] == '\0')){
			break;
		}
		tmp = strToInt(c);
		if ((c[i] < '0') || (c[i] > '9') || (tmp > INT_MAX) || (tmp == 0)){
			return false;
		}
		i++;
	}
	return true;
}

/*Function to check if the word is a palindrome or not*/
bool isPalindrome(char *p){
	int iLen = inputLen(p);
	int i = iLen - 1;

	for (int j = 0; i >= 0; i--, j++){
		if (p[i] != p[j]){
			return false;
		}
	} 
	return true;
}

/*Function to check if the number is a prime or not*/
bool isPrimeNumber(char *primen){
	unsigned primenInt = strToInt(primen);

	if (primenInt==2){
		return true;
	}
	if ((primenInt == 1) || (primenInt == 0) || (primenInt % 2 == 0)){ 
		return false;
	}

	for (unsigned i = 3; i*i <= primenInt; i += 2){
		if ((primenInt % i) == 0){
			return false;
		} 
	}
	return true;
}

/*
 * Function to check if there are any characters 
 * which are out of ASCII table (32-126) range.
 */
bool isError(char *e){
	for (int i = 0; e[i] != '\0'; i++){
		if ((e[i] > 126) || (e[i] < 32)){
			return true;
		}
	}
	return false;
}

int main (int argc, char __attribute__((unused))*argv[]){
	if (argc > 1){
		printf("\n \nThis program was developed by LM.\nIt has to be able to recognize" 
				"\nwhether the entered data are words,\nnubmers, palindromes or date.\n\n \n");
		return 0;
	}

	char inputText[102];
	int inputIn = scanf("%101s", inputText);

	while ((inputIn != EOF) && (inputIn != 0)){  
		/* If statement below calls the inputLen() function*/
		if (inputLen(inputText) > 100){
			printf("ERROR: It has to be less than 100 characters!\n");
			break;
		}
		/* In this if statement is called isError() function*/
		if (isError(inputText)){
			printf("ERROR: These characters are forbidden!\n");
			break;
		}

		/* In the following if statement is called the isNumber() function*/
		if (isNumber(inputText)){
			/*
			 * If the isNumber() function returns true, 
			 * isPrimeNumber() function is called in the next if statement
			 */
			if (isPrimeNumber(inputText))
				printf("number: %d (prime)\n",strToInt(inputText));
			else 
				printf("number: %d\n",strToInt(inputText));

		}
		/* In the following else if statement is called isDate() function */
		else if (isDate(inputText)){
			/*
			 * Calls the mktime() function included in time.h library 
			 * to check if the date is real or not. In function isDate(),
			 * there is a printf() called to print date format and the 
			 * corresponding day.
			 */
		}
		else {
			/*
			 * Calls isPalindrome() function to check if the word
			 * is a palindrome. 
			 */
			if(isPalindrome(inputText)){
				printf("word: %s (palindrome)\n", inputText);
			}
			else
				printf("word: %s\n", inputText);
		}
		inputIn = scanf("%102s", inputText);
	}
	return 0;
}

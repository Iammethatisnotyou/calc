/* See LICENSE for copyright and license details */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX_TOKENS 12
#define PI acos(-1.0L)
const char all_operators[] = {'+', '/', '-', 'x', '*', '%', '\0'};

void malloc_check(void *ptr){
	if (ptr == NULL) {
		perror("ERROR: Can't allocate memory");
		exit(2);
	}
}
void calculations(char *operator, long double *digits, unsigned int current_size){
	long double result = digits[0];
	for (int i = 0; i < strlen(operator); i++){
		if(operator[i] == '+'){
			result += digits[i+1];
		} else if (operator[i] == '-'){
			result -= digits[i+1];
		} else if (operator[i] == '*' || operator[i] == 'x'){
			if (operator[i+1] == '*'){ /* Exponents */
				long double base = (result != digits[0] ? result : digits[i]);
				result = powl(base, digits[i+1]);
				i += 2;
			} else {
				result *= digits[i+1];
			}
		} else if (operator[i] == '/'){
			if (operator[i+1] == '/'){
				result = (long long) result / (long long) digits[i+1];
				i++;
			} else {
				result /= digits[i+1];
			}
		} else if (operator[i] == '%'){
			result = (int) result % (int) digits[i+1];
		} else {
			exit(1);
		}
	}
	printf("%.4Lf\n", result);
}
void parser(char **argv){
	char *operator = malloc(1);
	unsigned int num_operator = 0, current_size = 0;
	long double *digits = malloc(sizeof(long double));

	malloc_check(digits);
	for (int i = 0; argv[i] != NULL; i++){
		for (int k = 0; k < strlen(argv[i]); k++){
			for (int j = 0; argv[i][j] != '\0'; j++){
				argv[i][j] = tolower(argv[i][j]);
			}
			if (strcmp(argv[i], "pi") == 0){
				current_size++;
				digits = realloc(digits, sizeof(long double) * current_size);
				malloc_check(digits);

				digits[current_size -1] = PI;
			} else if (strcmp(argv[i], "clear") == 0){
				system("clear");
			} else if (isalpha(argv[i][k])){
				printf("Invalid character: Probably letter\n");
				return;

			} else if (isdigit(argv[i][k]) || argv[i][k] == '.'){
				current_size++;
				digits = realloc(digits, sizeof(long double) * current_size);
				malloc_check(digits);
				digits[current_size -1] = strtod(argv[i], NULL);
			} else {
				for (int j = 0; j < strlen(all_operators); j++){
					if (all_operators[j] == argv[i][0]){
						num_operator++;
						operator = realloc(operator, num_operator);
						malloc_check(operator);
						operator[num_operator -1] = argv[i][0];
					}
				}
			}
		}
	}
	calculations(operator, digits, current_size);
}
int main(int argc, char *argv[]){
	bool quit = false;
	if (argc > 1){
		parser(argv+1);
		return 0;
	}
	while(!quit){
		char input[256];
		char lowercase_input[256];
		char **contents = malloc(sizeof(char *) * 12);
		malloc_check(contents);
		int token_count = 0;

		printf("> ");
		fgets(input, sizeof(input), stdin);
		
		for(int i = 0; input[i] != '\0'; i++){
			lowercase_input[i] = tolower(input[i]);
		}
		if (strcmp(lowercase_input, "exit\n") == 0){
			exit(0);
		}
		char *tokens = strtok(input, " \n");
		while (tokens && token_count < MAX_TOKENS){
			contents[token_count] = malloc(strlen(tokens) +1);
			strcpy(contents[token_count], tokens);
			token_count++;
			tokens = strtok(NULL, " \n");
		}
		parser(contents);
		for (int i = 0; i < token_count; i++){
			free(contents[i]);}
	}
	return 0;
}

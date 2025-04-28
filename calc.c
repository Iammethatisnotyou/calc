#include "config.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define PI acos(-1.0L)
const char all_operators[] = {'+', '/', '-', 'x', '*', '\0'};

void calculations(char *operator, long double *first_digit, unsigned int current_size){
	long double result = first_digit[0];
	for (int i = 0; i < strlen(operator); i++){
		if(operator[i] == '+'){
			result += first_digit[i+1];
		}
		else if (operator[i] == '-'){
			result -= first_digit[i+1];
		}
		else if (operator[i] == '*' || operator[i] == 'x'){
			if (operator[i+1] == '*'){ /* Exponents */
				result = powl(first_digit[i], first_digit[i+1]);
				i++;
			} else {
				result *= first_digit[i+1];
			}
		}
		else if (operator[i] == '/'){
			result /= first_digit[i+1];
		}

		else {
			exit(1);
		}
	}
	printf("%.4Lf\n", result);
}
void parser(char **argv){
	char *operator = malloc(1);
	unsigned int num_operator = 0;

	bool first_condition = false;
	long double *first_digit = malloc(sizeof(long double));
	unsigned int current_size = 0;

	if (first_digit == NULL){
		printf("ERROR: Can't Allocate Initial Memory");
		exit(1);
	}
	for (int i = 0; argv[i] != NULL; i++){
		for (int k = 0; k < strlen(argv[i]); k++){
			if (strcmp(argv[i], "PI") == 0){
				current_size++;
				first_digit = realloc(first_digit, sizeof(long double) * current_size);
				if (first_digit == NULL){
					printf("ERROR: Can't Allocate Enough Memory");
					exit(1);
				}
				first_digit[current_size -1] = PI;

			} else if (strcmp(argv[i], "clear") == 0){
				system("clear");
			} else if (isalpha(argv[i][k])){
				return;

			} else if (isdigit(argv[i][k]) || argv[i][k] == '.'){
				current_size++;
				first_digit = realloc(first_digit, sizeof(long double) * current_size);
				if (first_digit == NULL){
					printf("ERROR: Can't Allocate Enough Memory");
					exit(1);
				}
				first_digit[current_size -1] = strtod(argv[i], NULL);
			}
			for (int j = 0; j < strlen(all_operators); j++){
				if (all_operators[j] == argv[i][0]){
					num_operator++;
					operator = realloc(operator, num_operator);
					operator[num_operator -1] = argv[i][0];
				}
			}
		}
	}
	calculations(operator, first_digit, current_size);


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
		int token_count = 0;

		printf("> ");
		fgets(input, sizeof(input), stdin);
		
		for(int i = 0; i < sizeof(input); i++){
			lowercase_input[i] = tolower(input[i]);
		}
		if (strcmp(lowercase_input, "exit\n") == 0){
			quit = true;
			return 0;
		}

		char *tokens = strtok(input, " \n");
		if (!contents){
			printf("Memory Allocation Failed");
			exit(1);
		}
		while (tokens && token_count < 12){
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

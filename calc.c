/* See LICENSE for copyright and license details */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define VERSION 0.3
#define DIGIT "0123456789"
#define OPERATOR "+-*/x%\""
#define MAX_TOKENS 12
#define PI acos(-1.0L)

const char all_operators[] = {'+', '/', '-', 'x', '*', '%', '"', '\0'};
const char SPACE[] = " \f\n\r\t\v";

void malloc_check(void *ptr){
	if (ptr == NULL) {
		perror("ERROR: Can't allocate memory");
		exit(2);
	}
}
void calculations(char *operator, long double *digits, unsigned int current_size){
	long double result = digits[0];
	for (unsigned int i = 0, j = i+1; i < strlen(operator); i++, j++){
		if(operator[i] == '+'){
			result += digits[j];
		} else if (operator[i] == '-'){
			result -= digits[j];
		} else if (operator[i] == '*' || operator[i] == 'x'){
			if (operator[j] == '*'){ /* Exponents */
				long double base = (result != digits[0] ? result : digits[i]);
				result = powl(base, digits[j]);
				i += 2;
			} else {
				result *= digits[j];
			}
		} else if (operator[i] == '/'){
			if (operator[j] == '/'){
				result = (long long) result / (long long) digits[j];
				i++;
			} else {
				result /= digits[j];
			}
		} else if (operator[i] == '%'){
			result = (int) result % (int) digits[j];
		} else {
			exit(1);
		}
	}
	printf("%.4Lf\n", result);
}
/*void pemdas(char *operator, long double *digits, char **argv){
	 Goal, get operators and use pemdas order to re-organize them before doing calculations function
	for (int i = 0; i < argv[i]){
		for (int k = 0; k < argv[k]){
			if (argv[i][k] == '('){
			}
			else if (argv[i][k] == ')')
		}
	}
}*/
void parser(char **argv, int argc){
	char *operator = malloc(1);
	unsigned int num_operator = 0, current_size = 0;
	long double *digits = malloc(sizeof(long double));

	malloc_check(digits);
	for (int i = 0; i < argc; i++){
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
void formatting(char input[256], char ***contents, int *token_count){
	char *position = input;
	for (*token_count = 0; *token_count < MAX_TOKENS; *token_count++) {
		size_t len = strspn(position, DIGIT);
		if (len == 0){
			break;
		}
		*contents[*token_count] = malloc(len +1);
		strncpy(*contents[*token_count], position, len);
		*contents[*token_count][len] = '\0';
		position += len;
		
		size_t space_len = strspn(position, SPACE);
		position += space_len;
	}
}
int main(int argc, char *argv[]){
	bool quit = false;
	int token_count = 0;
	char **contents = calloc(sizeof(char *), MAX_TOKENS);
	
	if (argc == 2 && !strcmp("-v", argv[1])){
		printf("calc-%.2f\n", VERSION);
		exit(0);
	}
	if (argc > 1){
		char joined[256] = "";
		for (int i = 1; i < argc; i++) {
			strncat(joined, argv[i], 255);
		}
		joined[256] = '\0';
		printf("%s", joined);
		formatting(joined, &contents, &token_count);
		parser(contents, token_count);
		return 0;
	}
	while(!quit){
		char input[256];
		char *position = input;
		char lowercase_input[256];
		char **contents = calloc(sizeof(char *), MAX_TOKENS);
		unsigned int token_count;
		malloc_check(contents);

		printf("> ");
		fgets(input, sizeof(input), stdin);
		
		for(int i = 0; input[i] != '\0'; i++){
			lowercase_input[i] = tolower(input[i]);
		}
		if (strcmp(lowercase_input, "exit\n") == 0){
			exit(0);
		}
		for (token_count = 0; token_count < MAX_TOKENS; token_count++) { /* TURN THIS TO FUNCTION */
			size_t len = strspn(position, DIGIT);
			if (len == 0){
				len = strspn(position, OPERATOR);
			}
			contents[token_count] = malloc(len + 1);
			strncpy(contents[token_count], position, len);
			contents[token_count][len] = '\0';
			position += len;
			
			size_t space_len = strspn(position, SPACE);
			position += space_len;
		}
		parser(contents, token_count);
	}
	for (int i = 0; i < token_count; i++){
		free(contents[i]);
	}
	return 0;
}

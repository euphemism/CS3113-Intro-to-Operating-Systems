#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

char* read_line(char* buffer, size_t length, FILE* f)
/*
	From the tutorial at http://home.datacomm.ch/t_wolf/tw/c/getting_input.html
*/
{
	
	char* line;
	
	if (line = fgets(buffer, length, f))
	{
		
		size_t last = strlen(buffer) - 1;
		
		if (buffer[last] == '\n')
		{
			
			buffer[last] = '\0';
		}
		else
		{
			
			fscanf(f, "%*[^\n]");
			(void) fgetc(f);
		}
	}
	
	return line;
}
 
 char** explode(char* str, char delimiter)
 {
	 char** array = malloc(sizeof(char*));
	 int max_size = strlen(str);
	 int current_size = 0;
	 int offset = 0;
	 
	 printf("explode received %s", str);
	 
	 for (int i = 0; i < max_size; i++)
	 {
		 
		 printf(str);
		 
		 if (str[i] == delimiter)
		 {

			printf("hit delimiter.\n");
			 current_size++;
			 array = realloc(array, current_size * sizeof(char*));
			 array[current_size - 1] = malloc(((i - offset) + 2) * sizeof(char));
			 
			 for (int j = 0; j < (i - offset); j++)
			 {
				 
				 array[current_size - 1][j] = str[i - offset + j];
			 }
			 
			 offset = i + 1;
		 }
	 }
	 
	 return array;
 }
 
 char** collect_tokens(char* str)
 {

	char** result = (char**) malloc(sizeof(char*));
	int token_count = 1;
	result[token_count - 1] = strtok(str, " ");
	
	while (token != NULL)
	{
		
		token_count++;
		result = (char**) realloc(result, token_count * sizeof(char*));
		result[token_count - 1] = strtok(str, " ");
	}
	
	return result;
 }
 
int main(void)
{

	char buffer[BUFFER_SIZE];
	char *input;
	input = read_line(buffer, BUFFER_SIZE,  stdin);
	printf(&collect_tokens(input));

	exit(0); 
}
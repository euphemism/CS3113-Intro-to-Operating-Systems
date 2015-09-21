#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

char* read_line(char* buffer, size_t length, FILE* f)
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

int main(void)
{

	char buffer[BUFFER_SIZE];
	char *input;
	for (int i = 0; i < 10; i++)
	{
		
		input = read_line(buffer, BUFFER_SIZE,  stdin);
		printf(input + '\n');
	}
	exit(0);
}
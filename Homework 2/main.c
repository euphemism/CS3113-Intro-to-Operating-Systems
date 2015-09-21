#include <stdlib.h>
#include <string.h>
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

char *Readline(char *in) {
   char *cptr;

   if (cptr = fgets(in, MAX_LINE, stdin)) {
     /* kill preceding whitespace but leave \n so we're guaranteed to have something */
     while(*cptr == ' ' || *cptr == '\t') {
       cptr++;
     }
     return cptr;    
    } else {
     return 0;
   }
 }
 
int main(void)
{

	char buffer[BUFFER_SIZE];
	char *input;

	input = read_line(buffer, BUFFER_SIZE,  stdin);
	input >> cout;

	exit(0);
}
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define buffer_size 1024
#define READ 0
#define WRITE 1

int file_descriptor[2];
char buffer[buffer_size];

char quit[] = "q"; //Quit command string.
char input_buffer[buffer_size]; //Buffer for input.
char *line;

char** parseArguments(char* input)
{

	int count = 1;
	int i = 0;

	char * temp_string;

	for (i; i < strlen(input); i++)
	{

		if (input[i] == ' ')
			count++;
	}

	char **arguments = malloc((count + 1) * sizeof(char*));

	arguments[0] = strtok(input, " ");

	for (i = 1; i < count; i++)
		arguments[i] = strtok(NULL, " ");

	arguments[count] = NULL;

	for (i = 0; i <= count; i++)
		printf("ARG %d: %s\n", i, arguments[i]);
	return arguments;
}

/* From a tutorial online about reading input. */
char *read_line (char *buf, size_t length, FILE *f)
  /**** Read at most 'length'-1 characters from the file 'f' into
        'buf' and zero-terminate this character sequence. If the
        line contains more characters, discard the rest.
   */
{
  char *p;

  if (p = fgets (buf, length, f)) {
    size_t last = strlen (buf) - 1;

    if (buf[last] == '\n') {
      /**** Discard the trailing newline */
      buf[last] = '\0';
    } else {
      /**** There's no newline in the buffer, therefore there must be
            more characters on that line: discard them!
       */
      fscanf (f, "%*[^\n]");
      /**** And also discard the newline... */
      (void) fgetc (f);
    }
  }
  return p;
}

void error(void)
{

	printf("error.\n");
	exit(1);
}

int main(void)
{

//	int pipe_success;
	char* line;
/*
	pipe_success = pipe(file_descriptor);

	if (pipe_success == 0)
		printf("pipe successfully created.");
	else
	{

		printf("error creating pipe.");
		error();
	}
*/
	fflush(stdout);

	while (1)
	{

		char **args;
		char location[512];
		int return_value;
		int pid;
		int child_status;
		
		line = read_line(input_buffer, buffer_size, stdin);
		args = parseArguments(line);

		if (strcmp(args[0], quit) == 0)
			exit(0);
		
		strcpy(location, "/bin/");
		strcat(location, args[0]);
		printf("location: %s\n", location);

		if ((pid = fork()) == 0) //Child process
		{

			return_value = execvp(location, args);
			//We shouldn't be here.
			error();
		}
		else if (pid < 0) //Error
		{
			error();
		}
		else //Parent process
		{

			if ((pid = wait(&child_status)) == -1)
			{
				
				error();
			}
			else
			{
				
				if (WIFSIGNALED(child_status) != 0)
				{
					
				}
				else if (WIFEXITED(child_status) != 0)
				{
					
					printf("Done.\n");
				}
				else
					error();
			}
		}
	}
}
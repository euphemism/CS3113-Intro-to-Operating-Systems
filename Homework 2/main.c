#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define buffer_size 1024
#define READ 0
#define WRITE 1

int file_descriptor[2];
char buffer[buffer_size];

char quit[] = "quit"; //Quit command string.
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

void server(void)
{

	int client_message;
	int process_id;
	
	printf("server function.\n");	
	close(file_descriptor[1]);

	char **args;
	char location[512];
	int return_value;
	int pid;

	while(1)
	{

		client_message = read(file_descriptor[0], &buffer, buffer_size);

		printf("read: %s\n", buffer);
		switch (buffer[0])
		{
	
			case 'q': case 'Q':
				if (strcmp(buffer, quit) == 0)
					exit(0);

			default:
		
				if ((pid = fork()) == 0)
				{
			
					args = parseArguments(buffer);
			
					strcpy(location, "/bin/");
					strcat(location, args[0]);
					printf("location: %s\n", location);
	
					return_value = execvp(location, args);
	
					printf("not supposed to happen: %d\n", return_value);
				}
				else if (pid < 0)
					error();

				free(line);
				server();
		}

		sleep(1);
	}

	exit(0);
}

void client(void)
{

	int client_message;

	close(file_descriptor[0]);

	while(1)
	{

		line = read_line(input_buffer, buffer_size, stdin);
		client_message = write(file_descriptor[1], line, buffer_size);	
	}	
}

int main(void)
{

	int pipe_success;
	int process_id;

	printf("The server can perform the following services:\n");
	printf("(1) print time and date (using date)\n");
	printf("(2) print calendar for any month, with Sunday or Monday as ");
	printf("the first day of the week (using cal)\n");
	printf("(3) list the contents of the working or any other directory (using ls)\n");
	printf("(4) quit (using command '%s')\n", quit);

	pipe_success = pipe(file_descriptor);

	if (pipe_success == 0)
		printf("pipe successfully created.");
	else
	{

		printf("error creating pipe.");
		error();
	}

	fflush(stdout);

	if ((process_id = fork()) == 0)
		server();
	else if (process_id < 0)
		error();
	else
		client();
}
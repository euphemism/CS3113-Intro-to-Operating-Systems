#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 128
#define REMOVE_LAST_CHARACTER '*'
#define END_OF_LINE_MARKER '@'
#define DELETE_LAST_WORD '$'
#define ERASE_BUFFER '&'

bool read_entire_file;
char buffer[BUFFER_SIZE];
char read_character;
int buffer_index;

static pthread_cond_t print_line_cond;
static pthread_cond_t read_character_cond;
static pthread_cond_t stored_character_cond;

static pthread_mutex_t printing_line_lock;
static pthread_mutex_t reading_character_lock;

void* print_line(void* ptr)
{

	pthread_mutex_lock(&printing_line_lock);
	pthread_cond_wait(&print_line_cond, &printing_line_lock);

	while (!read_entire_file)
	{

		printf("%s\n", buffer);
		pthread_cond_signal(&print_line_cond);
		pthread_cond_wait(&print_line_cond, &printing_line_lock);
	}

	return NULL;
}

void* store_character(void* ptr)
{

	pthread_mutex_lock(&printing_line_lock);
	pthread_mutex_lock(&reading_character_lock);

	while (!read_entire_file)
	{

			switch(read_character)
			{

				case REMOVE_LAST_CHARACTER:
					buffer_index--;
					if (buffer_index < 0)
						buffer_index = 0;
					break;

				case ERASE_BUFFER:
					buffer_index = 0;
					break;

				case DELETE_LAST_WORD:
					for (;buffer_index > 0; buffer_index--)
						if (buffer[buffer_index] == ' ')
						{

							buffer_index++;
							break;
						}
					
					break;

				case END_OF_LINE_MARKER:
					buffer[buffer_index] = '\0';
					pthread_cond_signal(&print_line_cond);
					pthread_cond_wait(&print_line_cond, &printing_line_lock);
					buffer_index = 0;
					break;
					
				default:
					buffer[buffer_index++] = read_character;
			}

		pthread_cond_signal(&stored_character_cond);
		pthread_cond_wait(&read_character_cond, &reading_character_lock);
	}

	pthread_cond_signal(&print_line_cond);
	pthread_mutex_unlock(&printing_line_lock);

	return NULL;
}

int main(int arg_count, char* arg_values[])
{

	if (arg_count < 2)
		return -1;

	pthread_t printing_thread;
	pthread_t storing_thread;

	pthread_cond_init(&print_line_cond, NULL);
	pthread_cond_init(&read_character_cond, NULL);
	pthread_cond_init(&stored_character_cond, NULL);

	FILE* file_pointer = fopen(arg_values[1], "r");	
	if (file_pointer == NULL)
		return -1;

	buffer_index = 0;
	read_entire_file = false;

	if (pthread_mutex_init(&reading_character_lock, NULL) != 0) //Initialize reading mutex.
	{

		fputs("\n mutex init failed (reading_character_lock).\n", stderr);
		return -1;
	}

	if (pthread_mutex_init(&printing_line_lock, NULL) != 0) //Initialize printing mutex.
	{

		fputs("\n mutex init failed (printing_line_lock).\n", stderr);
		return -1;
	}

	pthread_mutex_lock(&reading_character_lock);
	pthread_create(&storing_thread, NULL, store_character, NULL);
	pthread_create(&printing_thread, NULL, print_line, NULL);

	while ((read_character = getc(file_pointer)) != EOF)
	{

		pthread_cond_signal(&read_character_cond);
		pthread_cond_wait(&stored_character_cond, &reading_character_lock);
	}

	fclose(file_pointer);
	read_entire_file = true;

	pthread_cond_signal(&read_character_cond);
	pthread_mutex_unlock(&reading_character_lock);

	pthread_join(storing_thread, NULL);	
	pthread_join(printing_thread, NULL);

	pthread_cond_destroy(&read_character_cond);
	pthread_cond_destroy(&stored_character_cond);
	pthread_cond_destroy(&print_line_cond);
	pthread_mutex_destroy(&reading_character_lock);
	pthread_mutex_destroy(&printing_line_lock);
}

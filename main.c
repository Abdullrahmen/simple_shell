#include "main.h"
#define INIT_BUFFER_SIZE 5

/**
* init_env - init the global environment variable
* @env: int from this env
*/
void init_env(char **env)
{
	
}


int handle_commands(char *str)
{
	printf("===\n%s===\n", str);
	return (1);
}

/**
* main - The main function
*/
int main(int argc, char **argv, char **env)
{
	char *buffer = NULL;
	size_t buffer_size = INIT_BUFFER_SIZE;
	int bytes_read = 0;
	int still_loop = 0;

	buffer = malloc(sizeof(char) * buffer_size);
	if (!buffer)
		return (0);

	still_loop = isatty(STDIN_FILENO);
	if (!still_loop) /*Non interactive mode*/
	{
		bytes_read = getline(&buffer, &buffer_size, stdin);
		if (bytes_read == -1)
		{
			free(buffer);
			return (0);
		}
		handle_commands(buffer);
	}

	while (still_loop)
	{
		write(1, "($) ", 4);
		bytes_read = getline(&buffer, &buffer_size, stdin);
		if (bytes_read == -1)
		{
			free(buffer);
			return (0);
		}
		still_loop = handle_commands(buffer);
	}
	free(buffer);
	return (0);
}


#include "main.h"

/**
* handle_commands - take a line of commands and handle them
* @commands: the line of commands
* @env: the environment list
* @alias: the alias list
* @program_name: the program name (argv[0])
* Return: zero if the commands tell the shell to exit (exit interactive loop)
* and one otherwise
*/
int handle_commands(char *commands, Item *env, Item *alias, char *program_name)
{
	size_t i = 0;
	Item *iter;

	iter = env;
	while (iter)
	{
		printf("%s=", iter->name);
		printf("%s\n", iter->value);
		iter = iter->next;
	}
	return (0);
}

/**
* main - The main function
*/
int main(int argc, char **argv, char **_env)
{
	char *buffer = NULL;
	size_t buffer_size = 0;
	ssize_t bytes_read = 0;
	int still_loop = 0;
	Item *env = NULL, *alias = NULL;

	alias = init_alias();
	env = init_env(_env);
	if (!alias || !env)
	{
		free_items_list(env);
		free_items_list(alias);
		return (0);
	}

	still_loop = isatty(STDIN_FILENO);
	if (!still_loop) /*Non interactive mode*/
	{
		bytes_read = getline(&buffer, &buffer_size, stdin);
		if (bytes_read == -1)
		{
			free_items_list(env);
			free_items_list(alias);
			free(buffer);
			return (0);
		}
		handle_commands(buffer, env, alias, argv[0]);
	}

	while (still_loop) /*Interactive mode*/
	{
		write(1, "($) ", 4);
		bytes_read = getline(&buffer, &buffer_size, stdin);
		if (bytes_read == -1)
		{
			free(buffer);
			return (0);
		}
		still_loop = handle_commands(buffer, env, alias, argv[0]);
	}
	free_items_list(env);
	free_items_list(alias);
	free(buffer);
	return (0);
}


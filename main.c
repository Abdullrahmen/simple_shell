#include "main.h"
/**
* __init__ - init env and alias
* @alias: alias
* @env: env
* @_env: environ
* Return: zero on failure
*/
int __init__(Item **alias, Item **env, char **_env)
{
	*alias = init_alias();
	*env = init_env(_env);
	_setenv_(env, EXIT_STATUS, "-1");
	_setenv_(env, LAST_EXIT_STATUS, "-1");
	if (!*alias || !*env)
	{
		free_items_list(*env);
		free_items_list(*alias);
		return (0);
	}
	return (1);
}
/**
* main - The main function
* @argc: argc
* @argv: argv
* @_env: environ
* Return: zero on success
*/
int main(__attribute__((unused))int argc, char **argv, char **_env)
{
	char *buffer = NULL;
	size_t buffer_size = 0, bytes_read = 0;
	unsigned int line_number = 1;
	int still_loop = 0, still_loop2 = 1;
	Item *env = NULL, *alias = NULL;

	if (!__init__(&alias, &env, _env))
		return (0);
	still_loop = isatty(STDIN_FILENO);
	if (!still_loop) /*Non interactive mode*/
		while (still_loop2)
		{
			bytes_read = getline(&buffer, &buffer_size, stdin);
			if (bytes_read == ULLONG_MAX)
			{
				free(buffer);
				break;
			}
			still_loop2 = handle_commands(buffer, &env, &alias, argv[0], line_number++);
			free(buffer);
			buffer = NULL;
		}
	while (still_loop) /*Interactive mode*/
	{
		write(1, "($) ", 4);
		bytes_read = getline(&buffer, &buffer_size, stdin);
		if (bytes_read == ULLONG_MAX)
		{
			write(1, "\n", 1);
			free(buffer);
			break;
		}
		still_loop = handle_commands(buffer, &env, &alias, argv[0], line_number++);
		free(buffer);
		buffer = NULL;
	}
	still_loop = _atoi_exit(env);
	free_items_list(env);
	free_items_list(alias);
	return (still_loop);
}


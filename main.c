#include "main.h"

/**
* handle_separators - handle separators (tell if the next command should be 
* executed or not)
* @prev_result: previous command result (-number is error, 1 is passed,
* prev_result shouldn't have the value zero but if it is then will return 0)
* @separator: the separator between the commands ex. && or || or ;
* Return: one if the next command should be executed and zero otherwise
*/
int handle_separators(int prev_result, char *separator)
{}

/**
* handle_command - handle one command only
* @command: the command
* @env: the environment list
* @alias: the alias list
* Return: zero if the command tell the shell to exit (exit interactive loop)
* and negative if there is an error (look at error ids)
* and one otherwise
*/
int handle_command(char **command, Item *env, Item *alias)
{}

/**
* filter_commands - filter the commands from comments and separators
* @commands: one line of commands
* @Return: list of commands separated by separators
* ex. ls;ls||ls -> [ls, ;, ls, ||, ls]
*/
char **filter_commands(char *commands)
{}

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
{}

/**
* main - The main function
*/
int main(int argc, char **argv, char **_env)
{
	char *buffer = NULL;
	size_t buffer_size = INIT_BUFFER_SIZE;
	ssize_t bytes_read = 0;
	int still_loop = 0;
	Item *env = NULL, *alias = NULL;

	alias = init_alias();
	env = init_env(_env);

	still_loop = isatty(STDIN_FILENO);
	if (!still_loop) /*Non interactive mode*/
	{
		bytes_read = getline(&buffer, &buffer_size, stdin);
		if (bytes_read == -1)
		{
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
	free(buffer);
	return (0);
}


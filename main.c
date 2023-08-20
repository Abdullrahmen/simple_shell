#include "main.h"
/**
* handle_command - handle one command only
* @command: the command
* @env: the environment list
* @alias: the alias list
* Return: zero if the command tell the shell to exit (exit interactive loop)
* and negative if there is an error (look at error ids)
* and one otherwise
*/
int handle_command(char *command, Item *env, Item *alias)
{
	char **argv = NULL;
	size_t i = 0;
	int command_type = 0;

	argv = malloc(sizeof(*argv) * get_n_tokens(command, ' ') + 1);
	argv[0] = _strtok(command, " ");
	while (argv[i])
	{
		printf("%s > ", argv[i]);
		++i;
		argv[i] = _strtok(NULL, " ");
	}
	printf("\n=============\n");
	fflush(stdout);
	free(argv);
	return (1);
}


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
	Item *commands_list = NULL, *iter_command;
	int command_result = 0, exec_next_command = 1;

	commands_list = filter_commands(commands, program_name);
	if (!commands_list)
		return (1);
	
	iter_command = commands_list;
	while (iter_command && exec_next_command)
	{
		if (!*iter_command->value)
			break;
		printf("=============\nSeparator:(%c)\nTokens:", *iter_command->name);
		name2value(&iter_command->value, env, alias);
		command_result = handle_command(iter_command->value, env, alias);
		if (!command_result)
		{
			free_items_list(commands_list);
			return (0);
		}
		/*if (command_result < 0)
			handle_error(iter_command->value, command_result, program_name);*/
		exec_next_command = handle_separators(command_result, *iter_command->name);
		iter_command = iter_command->next;
	}
	free_items_list(commands_list);
	return (1);
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
			write(1, "\n", 1);
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
			write(1, "\n", 1);
			free_items_list(env);
			free_items_list(alias);
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


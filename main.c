#include "main.h"

int get_command_type(char *first_token, Item *env)
{
	if (_strcmp(first_token, "exit"));
		return (COMMAND_OUR_BUILT_IN);
	return (COMMAND_PATH);
}

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
	char **argv = NULL, *path = NULL, **_env = NULL;
	size_t i = 0, argc = 0;
	int command_type = 0, command_result = 0;

	if (!command || !*command)
		return (1);
	argc = get_n_tokens(command, ' ') + 1;
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
	command_type = get_command_type(argv[0], env);

	path = _strdup(argv[0]);
	_env = items2str(env);
	switch(command_type)
	{/*
	case COMMAND_BUILT_IN:
		path = get_built_in_path(argv[0], env);
	case COMMAND_PATH:
		command_result = command_executer(path ,argv, argc, _env);
		break;*/
	case COMMAND_OUR_BUILT_IN:
		if (_strcmp(argv[0], "exit"))
			if (argv[1])
				set_item(env, EXIT_STATUS, argv[1]);
		break;
	}

	free(path);
	free(argv);
	i = 0;
	while (_env[i])
		free(_env[i++]);
	free(_env);
	if (command_type < 0)
		return (command_type);
	if (command_result)
		return (E_FILE_RETURN_E);
	if (_strcmp(argv[0], "exit"));
		return (0);
	return (command_result);
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

	set_item(env, EXIT_STATUS, "0");
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
		bytes_read = _getline(&buffer, &buffer_size, stdin);
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
		bytes_read = _getline(&buffer, &buffer_size, stdin);
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
	still_loop = atoi(get_item_value(env, EXIT_STATUS));
	free_items_list(env);
	free_items_list(alias);
	free(buffer);
	return (still_loop);
}


#include "main.h"
void _str_concat(char **dest, char *src)
{
	char *temp = NULL;
	size_t i = 0, len = 0, len2 = 0, len3 = 0;

	len = _strlen(*dest);
	len2 = _strlen(src);
	len3 = len + len2;
	temp = malloc(sizeof(char) * (len3 + 1));
	if (!temp)
		return;
	while (len3)
	{
		if (i < len)
			temp[i] = (*dest)[i];
		else
			temp[i] = src[i - len];
		++i;
		--len3;
	}
	temp[i] = '\0';
	free(*dest);
	*dest = temp;
}
/**
* 
*/
int check_command_type(char *first_token, Item *env, char **path)
{
	int is_command = 0;
	char *next_path = NULL, *g_path = NULL;
	size_t i = 0;

	if (first_token[0] == '/')
		*path = _strdup(first_token);
	else
	{
		g_path = _strdup(get_item_value(env, "PATH"));
		next_path = _strtok(g_path, ":");
		while (next_path)
		{
			*path = _strdup(next_path);
			_str_concat(path, "/");
			_str_concat(path, first_token);
			next_path = _strtok(NULL, ":");
			if (access(*path, F_OK && X_OK) == -1)
			{
				if (!next_path)
					break;
				free(*path);
			}
			else
				break;
		}
		is_command = 1;
		free(g_path);
	}

	if (access(*path, F_OK) == -1)
	{
		if (is_command)
			return (E_COMMAND_UNKNOWN);
		else
			return (E_PATH_NOT_EXIST);
	}
	if (access(*path, X_OK) == -1)
		return (E_PERMISSION_DENIED);
	return (1);
}

/*
1 - is our built in
0 - do nothing
-n - error
*/
int handle_our_built_in(char **argv, Item **env, Item **alias, int *is_exit)
{
	size_t i = 0;

	if (!_strcmp(argv[0], "exit"))
	{
		if (argv[1])
		{
			while (argv[1][i] >= '0' && argv[1][i] <='9')
				++i;
			if (argv[1][i])
				return (E_ILLEGAL_EXIT_NUMBER);
			_setenv_(env, EXIT_STATUS, argv[1]);
		}
		*is_exit = 1;
		return (1);
	}
	if (!_strcmp(argv[0], "env"))
	{
		if (!argv[1])
			_env_(*env);
		_setenv_(env, LAST_EXIT_STATUS, "0");
		return (1);
	}
	if (!_strcmp(argv[0], "cd"))
	{
		if (_cd_(*env, argv[1]) < 0)
			return (E_DIRECTORY_UNFOUND);
		_setenv_(env, LAST_EXIT_STATUS, "0");
		return (1);
	}
	if (!_strcmp(argv[0], "setenv"))
	{
		if (!argv[1] || !argv[2])
			return (E_INVALID_ARGUMENTS);
		if (!*argv[1] || !argv[2])
			return (E_INVALID_ARGUMENTS);
		if (_setenv_(env, argv[1], argv[2]))
			return (E_INVALID_ARGUMENTS);
		_setenv_(env, LAST_EXIT_STATUS, "0");
		return (1);
	}
	if (!_strcmp(argv[0], "alias"))
	{
		_alias_(alias, env, argv);
		return (1);
	}
	return (0);
}

/**
* handle_command - handle one command only
* @command: the command
* @env: the environment list
* @alias: the alias list
* @line_number: line number (used in errors)
* Return: zero if the command tell the shell to exit (exit interactive loop)
* and one otherwise
*/
int handle_command(char *command, Item **env, Item **alias, char *program_name, unsigned int line_number)
{
	char **argv = NULL, *path = NULL;
	size_t i = 0;
	int command_type = 0, command_result = 0, is_exit = 0, error_id = 0;

	if (!command || !*command)
		return (1);
	argv = malloc(sizeof(*argv) * (get_n_tokens(command, ' ') + 1));
	argv[0] = _strtok(command, " ");
	while (argv[i])
	{
		/*printf("%s > ", argv[i]);*/
		++i;
		argv[i] = _strtok(NULL, " ");
	}

	command_type = handle_our_built_in(argv, env, alias, &is_exit);
	if (!command_type)
	{
		command_type = check_command_type(argv[0], *env, &path);
		if (command_type >= 0)
		{

			command_result = command_executer(path, argv, env);
			free(path);
		}
	}

	/*printf("End result\n");
	printf("=============\n\n");*/
	fflush(stdout);
	if (command_type < 0)
		error_id = command_type;
	if (command_result)
		error_id = E_FILE_RETURN_E;
	if (is_exit)
	{
		free(argv);
		return (0);
	}
	handle_errors(argv, error_id, program_name, line_number, env);
	free(argv);
	if (error_id < 0)
		return (error_id);
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
int handle_commands(char *commands, Item **env, Item **alias, char *program_name, unsigned int line_number)
{
	Item *commands_list = NULL, *iter_command;
	int command_result = 0, exec_next_command = 1;

	commands_list = filter_commands(commands, program_name, line_number);
	if (!commands_list)
		return (1);
	iter_command = commands_list;
	while (iter_command && exec_next_command)
	{
		if (!*iter_command->value)
			break;
		/*printf("=============\nSeparator:(%c)\nTokens:", *iter_command->name);
		fflush(stdout);*/
		name2value(&iter_command->value, *env, *alias);
		command_result = handle_command(iter_command->value, env, alias, program_name, line_number);
		if (!command_result)
		{
			free_items_list(commands_list);
			return (0);
		}
			/*handle_error(iter_command->value, command_result, program_name);*/
		exec_next_command = handle_separators(command_result, *iter_command->name);
		iter_command = iter_command->next;
	}
	free_items_list(commands_list);
	return (1);
}

/**
* main - The main function
*/
int main(__attribute__((unused))int argc, char **argv, char **_env)
{
	char *buffer = NULL;
	size_t buffer_size = 0, bytes_read = 0;
	unsigned int line_number = 1;
	int still_loop = 0;
	Item *env = NULL, *alias = NULL;

	alias = init_alias();
	env = init_env(_env);
	_setenv_(&env, EXIT_STATUS, "0");
	_setenv_(&env, LAST_EXIT_STATUS, "0");
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
		if (bytes_read == ULLONG_MAX)
		{
			write(1, "\n", 1);
			free_items_list(env);
			free_items_list(alias);
			free(buffer);
			return (0);
		}
		handle_commands(buffer, &env, &alias, argv[0], line_number);
		free(buffer);
	}

	while (still_loop) /*Interactive mode*/
	{
		write(1, "($) ", 4);
		bytes_read = _getline(&buffer, &buffer_size, stdin);
		if (bytes_read == ULLONG_MAX)
		{
			write(1, "\n", 1);
			free_items_list(env);
			free_items_list(alias);
			free(buffer);
			return (0);
		}
		still_loop = handle_commands(buffer, &env, &alias, argv[0], line_number);
		++line_number;
		free(buffer);
		buffer = NULL;
	}
	still_loop = _atoi(get_item_value(env, EXIT_STATUS));
	free_items_list(env);
	free_items_list(alias);
	return (still_loop);
}


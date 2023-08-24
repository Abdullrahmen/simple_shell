#include "main.h"

/**
* check_path_access - checks the path access
* @path: the path
* @is_command: is the path command (used in errors)
* Return: one on success or error code otherwise
*/
int check_path_access(char *path, int is_command)
{
	if (access(path, F_OK) == -1)
	{
		if (is_command)
			return (E_COMMAND_UNKNOWN);
		else
			return (E_PATH_NOT_EXIST);
	}
	if (access(path, X_OK) == -1)
		return (E_PERMISSION_DENIED);
	return (1);
}

/**
* check_command_type - checks the command type
* @first_token: first token of the command
* @env: the Item environ
* @path: a path to change if it's a command
* Return: one on success or error code otherwise
*/
int check_command_type(char *first_token, Item *env, char **path)
{
	int is_command = 0;
	char *next_path = NULL, *g_path = NULL;

	if (first_token[0] == '/')
		*path = _strdup(first_token);
	else if (first_token[0] == '.' && first_token[1] == '/')
	{
		*path = _strdup(get_item_value(env, "PWD"));
		_str_concat(path, "/");
		_str_concat(path, &first_token[2]);
	}
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
	return (check_path_access(*path, is_command));
}
/**
* handle_our_built_in2 - handle our own built in functions (continue)
* @argv: argv
* @env: the environment
* @alias: the alias Item list
* Return: one if it's our built in functions
* zero if it isn't
* -number if it is but error raised (error code)
*/
int handle_our_built_in2(char **argv, Item **env, Item **alias)
{
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
	if (!_strcmp(argv[0], "unsetenv"))
	{
		if (!_unsetenv_(env, argv[1]))
			return (E_INVALID_ARGUMENTS);
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
* handle_our_built_in - handle our own built in functions
* @argv: argv
* @env: the environment
* @alias: the alias Item list
* @is_exit: will changed to one if the command tills the shell to exit
* Return: one if it's our built in functions
* zero if it isn't
* -number if it is but error raised (error code)
*/
int handle_our_built_in(char **argv, Item **env, Item **alias, int *is_exit)
{
	size_t i = 0;

	if (!_strcmp(argv[0], "exit"))
	{
		if (argv[1])
		{
			while (argv[1][i] >= '0' && argv[1][i] <= '9')
				++i;
			if (argv[1][i])
				return (E_ILLEGAL_EXIT_NUMBER);
			_setenv_(env, EXIT_STATUS, argv[1]);
		}
		else
			_setenv_(env, EXIT_STATUS, get_item_value(*env, LAST_EXIT_STATUS));
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
	return (handle_our_built_in2(argv, env, alias));
}


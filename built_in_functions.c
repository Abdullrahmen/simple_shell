#include "main.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/**
 * _cd_ - changes the current directory
 * @env: A list of environment variables
 * @directory: the directory required to change to it.
 * @program_name: the name of the program.
 * Return: 0 on success
 */
int _cd_(Item *env, char *directory, char *program_name)
{
	Item *env_iter = NULL;
	char cwd[1024];
	int value = 0;

	env_iter = env;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		write(STDERR_FILENO, "getcwd() error", 15);
		return (1);
	}
	if (directory == NULL || _strcmp(directory, "~") == 0)
		directory = get_item_value(env, "HOME");
	else if (_strcmp(directory, "-") == 0)
		directory = get_item_value(env, "OLDPWD");
	if (chdir(directory) == -1)
	{
		write(STDERR_FILENO, program_name, _strlen(program_name));
		write(STDERR_FILENO, ": 1", 3);
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, "cd", 2);
		write(STDERR_FILENO, ": can't cd to ", 14);
		write(STDERR_FILENO, directory, _strlen(directory));
		write(STDERR_FILENO, "\n", 1);
		return (1);
	}
	else
	{
		_setenv_(&env, "OLDPWD", cwd);
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			write(STDERR_FILENO, "getcwd() error", 15);
			return (1);
		}
	}
	return (0);
}

/**
 * _env_ - prints the environment variables
 * @env: the list of the environment variables
 * Return: 0 on success
 */
int _env_(Item *env)
{
	Item *env_iter = NULL;

	env_iter = env;
	while (env_iter)
	{
		write(STDOUT_FILENO, env_iter->name, _strlen(env_iter->name));
		write(STDOUT_FILENO, "=", 1);
		write(STDOUT_FILENO, env_iter->value, _strlen(env_iter->value));
		write(STDOUT_FILENO, "\n", 1);
		env_iter = env_iter->next;
	}
	return (0);
}

/**
 * _setenv_ - sets a new environment
 * @env: A list of items
 * @name: the name of the new variable
 * @value: the value of the new variable
 * Return: 0 on success;
 */
int _setenv_(Item **env, char *name, char *value)
{
	Item *env_iter = NULL;

	env_iter = *env;
	if (!env_iter || !name)
		write(STDERR_FILENO, "setenv ERROR", 13);
	env_iter->name = name;
	env_iter->value = value;
	while (env_iter)
	{
		if (_strcmp(name, env_iter->name))
		{
			env_iter = env_iter->next;
			continue;
		}
		env_iter->value = _strdup(value);
		return (0);
	}
	*env = add_node(*env, name, value);
	return (0);
}

/**
 * _unsetenv_ - unsets the environment variable
 * @env: A list of the environment variable
 * @name: the name of the variable
 * Return: 0 on success
 */
int _unsetenv_(Item *env, char *name)
{
	Item *env_iter = NULL;
	unsigned int i = 0;
	char *p;
	int env_changed;

	env_iter = env;
	if (!env_iter || !name)
		write(STDERR_FILENO, "unsetenv ERROR", 15);
	while (env_iter)
	{
		if (env_iter->name == name)
		{
			env_changed = delete_nodeint_at_index(&env, i);
			i = 0;
			env_iter = env;
			continue;
		}
		env_iter = env_iter->next;
		i++;
	}
	return (env_changed);
}

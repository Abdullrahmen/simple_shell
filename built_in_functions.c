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
 * Return: 0 on success
 */
int _cd_(Item *env, char *directory)
{
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (0);

	if (directory == NULL || _strcmp(directory, "~") == 0)
		directory = get_item_value(env, "HOME");
	else if (_strcmp(directory, "-") == 0)
		directory = get_item_value(env, "OLDPWD");

	if (chdir(directory) == -1)
		return (E_DIRECTORY_UNFOUND);

	_setenv_(&env, "OLDPWD", cwd);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (0);

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
 * _unsetenv_ - unsets the environment variable
 * @env: A list of the environment variable
 * @name: the name of the variable
 * Return: one on success and 0 on failure
 */
int _unsetenv_(Item **env, char *name)
{
	Item *env_iter = NULL;
	unsigned int i = 0;
	int env_changed = 0;

	env_iter = *env;
	if (!env_iter || !name)
		return (0);
	while (env_iter)
	{
		if (!_strcmp(env_iter->name, name))
		{
			env_iter = env_iter->next;
			env_changed = delete_nodeint_at_index(env, i);
			continue;
		}
		env_iter = env_iter->next;
		++i;
	}
	return (env_changed);
}

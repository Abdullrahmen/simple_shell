#include "main.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/**
 * _cd_ - changes the current directory
 * @directory: the directory required to change to it.
 * @program_name: the name of the program.
 * Return: 0 on success
 */
int _cd_(char *directory, char *program_name)
{
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd() error");
		return (1);
	}
	if (directory == NULL || _strcmp(directory, "~") == 0)
		directory = get_item_value("HOME");
	else if (_strcmp(directory, "-") == 0)
		directory = get_item_value("OLDPWD");
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
		_setenv_("OLDPWD", cwd);
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("getcwd() error");
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
int _setenv_(Item *env, char *name, char *value)
{
	Item *env_iter = NULL;

	env_iter = malloc(sizeof(Item));
	if (!env_iter)
		perror("setenv ERROR");
	env_iter = env;
	env_iter->name = name;
	env_iter->value = value;
	if (env_iter)
	{
		add_node(&env_iter, env_iter->name, env_iter->value);
	}
	return (0);
}

/**
 * add_node - adds a new node at the beginning of a list_t list.
 * @head: the head of the list
 * @name: the name of the new variable
 * @value: the value of the new variable
 * Return: the address of the new element
 */
Item *add_node(Item **head, char *name, char *value)
{
	Item *new_head = malloc(sizeof(Item));

	if (!head || !new_head)
		return (NULL);
	if (name)
	{
		new_head->name = _strdup(name);
		new_head->value = _strdup(value);
		if (!new_head->name  || !new_head->value)
		{
			free(new_head);
			return (NULL);
		}
	}
	new_head->next = *head;
	*head = new_head;
	return (new_head);
}

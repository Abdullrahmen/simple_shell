#include "main.h"

/**
* init_alias - Initialize an Item struct alias list
* Return: pointer to the first node or NULL if error
*/
Item *init_alias()
{
	Item *alias = NULL;

	alias = malloc(sizeof(Item));
	if (!alias)
		return (NULL);
	alias->name = _strdup("");
	alias->value = _strdup("");
	alias->next = NULL;
	return (alias);
}

/**
* alias_2 - _alias_ continue
* @argv: argv
* @i: i
* @str_iter: str_iter
* @name: name
* @value: value
* @alias: alias
* @is_error: is error
*/
void alias_2(char **argv, size_t i, char *str_iter, char **name,
		char **value, Item **alias, int *is_error)
{
	size_t j = 0;

	*name = copy_till_delim(str_iter, '=');
	if (*name)
	{
		while (str_iter[j] != '=')
			++j;
		if (str_iter[j + 1])
		{
			*value = _strdup(&str_iter[j + 1]);
			if (*value)
				_setenv_(alias, *name, *value);
			else
				*is_error = 1;
			free(*value);
		}
	}
	else
	{
		*value = get_item_value(*alias, argv[i]);
		if (*value)
		{
			write(STDOUT_FILENO, argv[i], _strlen(argv[i]));
			write(STDOUT_FILENO, "='", 2);
			write(STDOUT_FILENO, *value, _strlen(*value));
			write(STDOUT_FILENO, "'\n", 2);
		}
		else
		{
			write(STDERR_FILENO, "alias: ", 7);
			write(STDERR_FILENO, argv[i], _strlen(argv[i]));
			write(STDERR_FILENO, " not found\n", 11);
			*is_error = 1;
		}
	}
	free(*name);
}

/**
* _alias_ - handle alias built in command
* @alias: alias
* @env: the list of the environment variables
* @argv: argv (tokens of the command)
* Return: 0 on success
*/
int _alias_(Item **alias, Item **env, char **argv)
{
	Item *iter = NULL;
	char *str_iter = NULL, *name = NULL, *value = NULL;
	size_t i = 1;
	int is_error = 0;

	if (_strcmp(argv[0], "alias"))
		return (1);

	if (!argv[1] || !argv[1][0]) /*alias only*/
	{
		iter = *alias;
		while (iter && iter->name[0])
		{
			write(STDOUT_FILENO, iter->name, _strlen(iter->name));
			write(STDOUT_FILENO, "='", 2);
			write(STDOUT_FILENO, iter->value, _strlen(iter->value));
			write(STDOUT_FILENO, "'\n", 2);
			iter = iter->next;
		}
		return (0);
	}

	i = 1;
	str_iter = argv[i];
	while (str_iter)
	{
		alias_2(argv, i, str_iter, &name, &value, alias, &is_error);
		++i;
		str_iter = argv[i];
	}
	if (is_error)
		_setenv_(env, LAST_EXIT_STATUS, "1");
	else
		_setenv_(env, LAST_EXIT_STATUS, "0");
	return (0);
}



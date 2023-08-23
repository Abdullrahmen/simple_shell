#include "main.h"

/**
 * _env_ - prints the environment variables
 * @env: the list of the environment variables
 * Return: 0 on success
 */
int _alias_(Item **alias, Item **env, char **argv)
{
	Item *iter = NULL;
	char *str_iter = NULL, *name = NULL, *value = NULL;
	size_t i = 1, j = 0;
	int is_error = 0;

	if (_strcmp(argv[0], "alias"))
		return (1);
	
	if (!argv[1] || !argv[1][0]) /*alias only*/
	{
		iter = *alias;
		while (iter)
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
		name = copy_till_delim(str_iter, '=');
		if (name)
		{
			j = 0;
			while (str_iter[j] != '=')
				++j;
			if (str_iter[j + 1] == '\'' && str_iter[j + 2])
			{
				value = copy_till_delim(&str_iter[j + 2], '\'');
				if (value)
					_setenv_(alias, name, value);
				else
					is_error = 1;
				free(value);
			}
		}
		else
		{
			value = get_item_value(*alias, argv[i]);
			if (value)
			{
				write(STDOUT_FILENO, argv[i], _strlen(argv[i]));
				write(STDOUT_FILENO, "='", 2);
				write(STDOUT_FILENO, value, _strlen(value));
				write(STDOUT_FILENO, "'\n", 2);
			}
			else
			{
				write(STDERR_FILENO, "alias: ", 7);
				write(STDERR_FILENO, argv[i], _strlen(argv[i]));
				write(STDERR_FILENO, " not found\n", 11);
				is_error = 1;
			}
		}
		free(name);
		++i;
		str_iter = argv[i];
	}
	if (is_error)
		_setenv_(env, LAST_EXIT_STATUS, "1");
	else
		_setenv_(env, LAST_EXIT_STATUS, "0");
	return (0);
}

/**
 * add_node - adds a new node at the beginning of a list_t list.
 * @head: the head of the list
 * @name: the name of the new variable
 * @value: the value of the new variable
 * Return: the address of the new head
 */
Item *add_node(Item *head, char *name, char *value)
{
	Item *new_head = malloc(sizeof(Item));

	if (!head || !new_head)
	{
		free(new_head);
		return (NULL);
	}

	new_head->name = _strdup(name);
	new_head->value = _strdup(value);
	if (!new_head->name  || !new_head->value)
	{
		free(new_head->name);
		free(new_head->value);
		free(new_head);
		return (NULL);
	}
	new_head->next = head;
	return (new_head);
}


/**
 * _setenv_ - sets a new environment
 * @env: A list of items
 * @name: the name of the new variable
 * @value: the value of the new variable
 * Return: 0 on success or -n in failure;
 */
int _setenv_(Item **env, char *name, char *value)
{
	Item *env_iter = NULL;

	if (!name || !value)
		return (E_INVALID_ARGUMENTS);

	env_iter = *env;
	while (env_iter)
	{
		if (_strcmp(name, env_iter->name))
		{
			env_iter = env_iter->next;
			continue;
		}
		free(env_iter->value);
		env_iter->value = _strdup(value);
		return (0);
	}
	*env = add_node(*env, name, value);
	return (0);
}
char *copy_from_delim(char *str, char delim)
{
	char *copied;
	size_t i = 0, j = 1;

	while (str[i] && str[i] != delim)
		++i;
	if (!str[i])
		return (NULL);
	while (str[i + j])
		++j;
	
	copied = malloc(sizeof(*copied) * j);
	if (!copied)
		return (NULL);

	j = 1;
	while (str[i + j])
	{
		copied[j - 1] = str[i + j];
		++j;
	}
	copied[j - 1] = '\0';
	return (copied);
}

char *copy_till_delim(char *str, char delim)
{
	char *copied;
	size_t i = 0;

	while (str[i] && str[i] != delim)
		++i;

	if (!str[i])
		return (NULL);

	copied = malloc(sizeof(*copied) * (i + 1));
	if (!copied)
		return (NULL);

	i = 0;
	while (str[i] != delim)
	{
		copied[i] = str[i];
		++i;
	}
	copied[i] = '\0';
	return (copied);
}

/**
* init_env - Initialize an Item struct environment list from char **
* note:the pointer must has a value even if it points to an empty Item
* @_env: the char ** env
* Return: pointer to the first node or NULL if error
*/
Item *init_env(char **_env)
{
	Item *env = NULL, *iter = NULL;
	size_t i = 0;

	env = malloc(sizeof(Item));
	if (!env)
		return (NULL);
	env->name = NULL;
	env->value = NULL;
	env->next = NULL;

	iter = env;
	while (_env[i])
	{
		iter->name = copy_till_delim(_env[i], '=');
		iter->value = copy_from_delim(_env[i], '=');
		if (!iter->value || !iter->name)
		{
			free_items_list(env);
			return (NULL);
		}
		if (_env[i + 1])
		{
			iter->next = malloc(sizeof(Item));
			if (!iter->next)
			{
				free_items_list(env);
				return (NULL);
			}
			iter = iter->next;
		}
		++i;
	}
	iter->next = NULL;
	return (env);
}

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
* free_items_list - free an items linked list
* @items: the list
*/
void free_items_list(Item *items)
{
	Item *iter = items;

	free(iter->name);
	free(iter->value);
	free(iter);
	while (iter->next)
	{
		iter = iter->next;
		free(iter->name);
		free(iter->value);
		free(iter);
	}
}


#include "main.h"

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
	alias->name = NULL;
	alias->value = NULL;
	alias->next = NULL;
	return (alias);
}

/**
* free_items_list - free an items linked list
* @items: the list
*/
void free_items_list(Item *items)
{
	free(items);
	free(items->name);
	free(items->value);
	while (items->next)
	{
		items = items->next;
		free(items);
		free(items->name);
		free(items->value);
	}
}


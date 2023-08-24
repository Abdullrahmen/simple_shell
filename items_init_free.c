#include "main.h"
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
* free_items_list - free an items linked list
* @items: the list
*/
void free_items_list(Item *items)
{
	Item *iter = items;

	if (!items)
		return;
	iter = items;
	items = items->next;
	free(iter->name);
	free(iter->value);
	free(iter);
	while (items)
	{
		iter = items;
		items = items->next;
		free(iter->name);
		free(iter->value);
		free(iter);
	}
}

/**
* skip_spaces - skip spaces
* @str:string
* Return: a pointer to the character after the last space
*/
char *skip_spaces(char *str)
{
	size_t i = 0;

	while (str[i] == ' ')
		++i;
	return (&str[i]);
}


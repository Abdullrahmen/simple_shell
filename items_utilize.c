#include "main.h"

/**
* name2value_2 - name2value continue
* @i:i
* @len:len
* @token:token
* @value:value
* @env:env
* @copied_str:copied string
* @str:str
* Return: zero to continue
*/
int name2value_2(size_t *i, size_t *len, char *token, char **value,
		Item *env, char **copied_str, char **str)
{
	char *temp_str = NULL, *sub_str = NULL;

	if (!token[1])
	{
		*i += _strlen(token);
		return (0);
	}
	if (token[1] == '$' && !token[2])
		*value = uint2str(getpid());
	else if (token[1] == '?' && !token[2])
		*value = _strdup(get_item_value(env, LAST_EXIT_STATUS));
	else
		*value = _strdup(get_item_value(env, &token[1]));
	if (*value)
	{
		*len = _strlen(*copied_str) - _strlen(token)
						+ _strlen(*value) + 1;
		temp_str = malloc(sizeof(char) * (*len));
		temp_str[0] = '\0';

		sub_str = _substr(*str, *i);
		_str_concat(&temp_str, sub_str);
		_str_concat(&temp_str, *value);
		_str_concat(&temp_str, &((*str)[*i + _strlen(token)]));
		free(sub_str);

		free(*str);
		*str = temp_str;

		*i += _strlen(*value);
		free(*copied_str);
		free(*value);
		*copied_str = _strdup(*str);
		return (0);
	}
	return (1);
}

void alias_name2value(char **str, char **copied_str, size_t *len, char *token,
						char *value, size_t *i)
{
	char *temp_str = NULL;

	*len = _strlen(*str) - _strlen(token) + _strlen(value) + 1;
	temp_str = malloc(sizeof(char) * *len);
	temp_str[0] = '\0';
	_str_concat(&temp_str, value);
	_str_concat(&temp_str, &((*str)[_strlen(token)]));
	free(*str);
	*str = temp_str;
	*i += _strlen(value) + 1;
	free(*copied_str);
	*copied_str = _strdup(*str);
}

/**
* name2value - convert all env and alias Items in the string to their values
* @str: the string
* @env: the environment
* @alias: the alias list
*/
void name2value(char **str, Item *env, Item *alias)
{
	char *value = NULL, *token = NULL, *copied_str = NULL;
	size_t i = 0, len = 0, len2 = 0;

	if (!str || !*str || !**str)
		return;
	copied_str = _strdup(*str);
	len2 = _strlen(copied_str);
	token = _strtok(copied_str, " ");
	if (!token)
		return;

	value = get_item_value(alias, token);
	if (value)
		alias_name2value(str, &copied_str, &len, token, value, &i);
	else
		if (_strlen(token) >= len2)
		{
			free(copied_str);
			return;
		}
		else
			copied_str[_strlen(token)] = ' ';
	len2 = _strlen(copied_str);
	while (1)
	{
		if (i >= len2)
			break;
		token = _strtok(&(copied_str[i]), " ");
		if (!token)
			break;
		if (token[0] == '$')
			if (!name2value_2(&i, &len, token, &value, env,
							&copied_str, str))
				continue;
		i += _strlen(token) + 1;
	}
	free(copied_str);
}

/**
 * get_item_value - gets the value of the name.
 * @items: A list of items
 * @name: the name that required to get its value.
 * Return: the value of the variable
 */
char *get_item_value(Item *items, char *name)
{
	Item *items_iter = NULL;

	items_iter = items;
	if (name == NULL)
		return (NULL);
	while (items_iter)
	{
		if (_strcmp(items_iter->name, name) == 0)
			return (items_iter->value);
		items_iter = items_iter->next;
	}
	return (NULL);
}

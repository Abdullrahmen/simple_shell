#include "main.h"

char *get_item_value(Item *items, char *name)
{
	Item *items_iter = NULL;

	items_iter = items;
	if (!name)
		return (NULL);
	while (items_iter)
	{
		if (!_strcmp(items_iter->name, name))
			return (items_iter->value);
		else
			items_iter = items_iter->next;
	}
	return (NULL);
}

char **items2str(Item *items)
{
	Item *iter;
	char **strings;
	size_t i = 0, len = 0;

	if (!items)
		return (NULL);
	iter = items;
	strings = malloc(sizeof(*strings) * (get_items_len(items) + 1));
	while (iter)
	{
		len = _strlen(iter->name) + _strlen(iter->value) + 2;
		strings[i] = malloc(len);
		strings[i][0] = '\0';
		_strcat(strings[i], iter->name);
		_strcat(strings[i], "=");
		_strcat(strings[i], iter->value);
		++i;
		iter = iter->next;
	}
	strings[i] = NULL;
	return (strings);
}

size_t get_items_len(Item *items)
{
	Item *iter;
	size_t len = 0;

	if (!items || (!items->value && !items->name && !items->next))
		return (0);
	iter = items;
	while (iter)
	{
		++len;
		iter = iter->next;
	}
	return (len);
}

void name2value(char **str, Item *env, Item *alias)
{
	Item *alias_iter = NULL, *env_iter = NULL;
	char *value = NULL, *token = NULL, *temp_str = NULL, *sub_str = NULL, *copied_str = NULL;
	int is_equal = 0;
	size_t i = 0, len = 0;

	if (!str || !*str || !**str)
		return;

	alias_iter = alias;
	env_iter = env;
	copied_str = _strdup(*str);
	token = _strtok(copied_str, " ");
	if (!token)
		return;
	value = get_item_value(alias, token);

	if (value)
	{
		len = _strlen(*str) - _strlen(token) + _strlen(value) + 1;
		temp_str = malloc(sizeof(char) * len);
		temp_str[0] = '\0';

		_strcat(temp_str, value);
		_strcat(temp_str, &((*str)[_strlen(token)]));

		free(*str);
		*str = temp_str;

		i += _strlen(value) + 1;
		free(copied_str);
		copied_str = _strdup(*str);
	}
	while (1)
	{
		token = _strtok(&(copied_str[i]), " ");
		if (!token)
			break;
		if (token[0] == '$')
		{
			if (!token[1])
			{
				i += _strlen(token);
				continue;
			}
			value = get_item_value(env, &token[1]);
			if (value)
			{
				len = _strlen(copied_str) - _strlen(token) + _strlen(value) + 1;
				temp_str = malloc(sizeof(char) * len);
				temp_str[0] = '\0';

				sub_str = _substr(*str, i);
				_strcat(temp_str, sub_str);
				_strcat(temp_str, value);
				_strcat(temp_str, &((*str)[i + _strlen(token)]));
				free(sub_str);

				free(*str);
				*str = temp_str;

				i += _strlen(value);
				free(copied_str);
				copied_str = _strdup(*str);
				continue;
			}
		}
		i += _strlen(token) + 1;
	}
	free(copied_str);
}

size_t get_n_tokens(char *str, char delim)
{
	size_t n = 1, i = 0;

	while (str[i])
	{
		if (str[i] == delim)
		{
			while (str[i] == delim)
				++i;
			++n;
		}
		else
			++i;
	}
	return (n);
}


/**
* handle_separators - handle separators (tell if the next command should be 
* executed or not)
* @prev_result: previous command result (-number is error, 1 is passed,
* prev_result shouldn't have the value zero but if it is then will return 0)
* @separator: the separator between the commands ex. && or || or ;
* Return: one if the next command should be executed and zero otherwise
*/
int handle_separators(int prev_result, char separator)
{
	return (1);
}

char *skip_spaces(char *str)
{
	size_t i = 0;
	while (str[i] == ' ')
		++i;
	return (&str[i]);
}

/**
* filter_commands - filter the commands from comments and separators
* @commands: one line of commands
* @program_name: argv[0] used in some error massages
* @Return: list of commands separated by separators
* or NULL if error
* ex. ls;ls||ls -> [ls, ;, ls, ||, ls]
*/
Item *filter_commands(char *commands, char *program_name)
{
	Item *commands_list, *iter2;
	char *iter, error_msg[_strlen(program_name) + 100];
	size_t start = 0, end = 0;

	_strcpy(error_msg, program_name);
	_strcat(error_msg, ": 1: Syntax error: \"");
	iter = skip_spaces(commands);
	if (!iter[0])
		return (NULL);
	if (iter[0] == '#')
		return (NULL);

	commands_list = malloc(sizeof(Item));
	if (!commands_list)
	{
		free_items_list(commands_list);
		return (NULL);
	}
	iter2 = commands_list;
	while (iter[end])
	{
		if ((iter[start] == ';' && iter[start + 1] == ';') ||
			(iter[start] == '|' && iter[start + 1] == '|') ||
			(iter[start] == '&' && iter[start + 1] == '&'))
		{
			write(STDERR_FILENO, _strcat(error_msg, &iter[start]), _strlen(error_msg) + 2);
			write(STDERR_FILENO, "\" unexpected\n", 13);
			return (NULL);
		}
		if (iter[start] == ';' || iter[start] == '|' || iter[start] == '&')
		{
			write(STDERR_FILENO, _strcat(error_msg, &iter[start]), _strlen(error_msg) + 1);
			write(STDERR_FILENO, "\" unexpected\n", 13);
			return (NULL);
		}

		iter2->name = malloc(sizeof(char));
		if (!iter2->name)
		{
			free_items_list(commands_list);
			return (NULL);
		}
		*iter2->name = ' ';
		iter2->value = NULL;
		iter2->next = NULL;

		while (iter[end] != ';' && iter[end] != '|' &&
			iter[end] != '&' && iter[end] != '\n' && iter[end] != '#')
			++end;

		if (iter[end] == '|')
			if (iter[end + 1] == '|')
				*iter2->name = '|';
			else
			{
				free_items_list(commands_list);
				write(STDERR_FILENO, "| is not supported yet\n", 24);
				return (NULL);
			}
		if (iter[end] == '&')
			if (iter[end + 1] == '&')
				*iter2->name = '&';
			else
			{
				free_items_list(commands_list);
				write(STDERR_FILENO, "& is not supported yet\n", 24);
				return (NULL);
			}
		if (iter[end] == ';')
			if(iter[end + 1] == ';')
			{
				free_items_list(commands_list);
				write(STDERR_FILENO, _strcat(error_msg, ";;\" unexpected\n"), _strlen(error_msg) + 15);
				return (NULL);
			}
			else
			*iter2->name = ';';
		
		if (iter[end] == '#' || iter[end] == '\n')
		{
			iter2->value = _substr(&iter[start], end - start);
			break;
		}
		else
		{
			iter2->value = _substr(&iter[start], end - start);
			iter2->next = malloc(sizeof(Item));
			iter2 = iter2->next;
		}
		
		++end;
		if ((iter[end] == '|' && iter[end - 1] == '|') ||
			(iter[end] == '&' && iter[end - 1] == '&'))
			++end;
		while (iter[end] == ' ')
			++end;
		start = end;
	}
	*iter2->name = ' ';
	return (commands_list);
}

char *_substr(char *str, size_t bytes)
{
	char *substr = NULL;
	size_t i = 0;

	while (str[i] && i < bytes)
		++i;
	substr = malloc(sizeof(char) * i + 1);
	i = 0;
	while (str[i] && i < bytes)
	{
		substr[i] = str[i];
		++i;
	}
	substr[i] = '\0';
	return (substr);
}


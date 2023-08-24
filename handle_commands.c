#include "main.h"
/**
* check_format - filter_commands continue
* @error_msg:error message
* @iter: iter
* @start: start
* @iter2: iter2
* @commands_list: commands list
* Return: zero on failure
*/
int check_format(char **error_msg, char *iter, size_t start,
				Item *iter2, Item *commands_list)
{
	if ((iter[start] == ';' && iter[start + 1] == ';') ||
		(iter[start] == '|' && iter[start + 1] == '|') ||
		(iter[start] == '&' && iter[start + 1] == '&'))
	{
		_str_concat(error_msg, &iter[start]);
		write(STDERR_FILENO, *error_msg, _strlen(*error_msg) + 2);
		write(STDERR_FILENO, "\" unexpected\n", 13);
		free(*error_msg);
		return (0);
	}
	if (iter[start] == ';' || iter[start] == '|' || iter[start] == '&')
	{
		_str_concat(error_msg, &iter[start]);
		write(STDERR_FILENO, *error_msg, _strlen(*error_msg) + 1);
		write(STDERR_FILENO, "\" unexpected\n", 13);
		free(*error_msg);
		return (0);
	}
	iter2->name = malloc(sizeof(char));
	if (!iter2->name)
	{
		free_items_list(commands_list);
		free(*error_msg);
		return (0);
	}
	*(iter2->name) = ' ';
	iter2->value = NULL;
	iter2->next = NULL;

	return (1);
}

/**
* check_format2 - filter_commands continue
* @iter: iter
* @iter2: iter2
* @commands_list: commands list
* @error_msg: error message
* @end: end
* Return: zero on failure
*/
int check_format2(char *iter, Item *iter2, Item *commands_list,
					char **error_msg, size_t end)
{
	if (iter[end] == '|')
	{
		if (iter[end + 1] == '|')
			*iter2->name = '|';
		else
		{
			free_items_list(commands_list);
			write(STDERR_FILENO, "| is not supported yet\n", 24);
			free(*error_msg);
			return (0);
		}
	}
	if (iter[end] == '&')
	{
		if (iter[end + 1] == '&')
			*iter2->name = '&';
		else
		{
			free_items_list(commands_list);
			write(STDERR_FILENO, "& is not supported yet\n", 24);
			free(*error_msg);
			return (0);
		}
	}
	if (iter[end] == ';')
	{
		if (iter[end + 1] == ';')
		{
			free_items_list(commands_list);
			_str_concat(error_msg, ";;\" unexpected\n");
			write(STDERR_FILENO, *error_msg, _strlen(*error_msg) + 15);
			free(*error_msg);
			return (0);
		}
		else
			*iter2->name = ';';
	}
	return (1);
}

/**
* init_format - filter commands continue
* @iter:iter
* @error_msg: error message
* @program_name: program name
* @commands: commands
* @commands_list: commands list
* @line_number: line number
* @iter2: iter2
* Return: zero on failure
*/
int init_format(char **iter, char **error_msg, char *program_name,
		char *commands, Item **commands_list,
		unsigned int line_number, Item **iter2)
{
	*iter = uint2str(line_number);
	*error_msg = malloc(sizeof(char) * (_strlen(program_name) + _strlen(*iter)));
	if (!*error_msg)
		return (0);
	_strcpy(*error_msg, program_name);
	_str_concat(error_msg, ": ");
	_str_concat(error_msg, *iter);
	_str_concat(error_msg, ": Syntax error: \"");
	free(*iter);
	*iter = skip_spaces(commands);
	if (!(*iter)[0])
	{
		free(*error_msg);
		return (0);
	}
	if ((*iter)[0] == '#')
	{
		free(*error_msg);
		return (0);
	}

	*commands_list = malloc(sizeof(Item));
	if (!*commands_list)
	{
		free_items_list(*commands_list);
		return (0);
	}
	*iter2 = *commands_list;
	return (1);
}

/**
* filter_commands - filter the commands from comments and separators
* @commands: one line of commands
* @program_name: argv[0] used in some error massages
* @line_number: line number (used in errors)
* Return: linked list of commands where name is the separator and the value is
* the command
*/
Item *filter_commands(char *commands, char *program_name,
					unsigned int line_number)
{
	Item *commands_list, *iter2;
	char *iter, *error_msg;
	size_t start = 0, end = 0;

	if (!init_format(&iter, &error_msg, program_name, commands,
					&commands_list, line_number, &iter2))
		return (NULL);
	while (iter[end])
	{
		if (!check_format(&error_msg, iter, start, iter2, commands_list))
			return (NULL);

		while (iter[end] != ';' && iter[end] != '|' &&
		iter[end] != '&' && iter[end] != '\n' && iter[end] != '#')
			++end;
		if (!check_format2(iter, iter2, commands_list,
						&error_msg, end))
			return (NULL);
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
	free(error_msg);
	return (commands_list);
}


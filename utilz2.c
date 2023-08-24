#include "main.h"
/**
* get_n_tokens - get the number of tokens
* @str:string
* @delim: delim
* Return: number of tokens
*/
size_t get_n_tokens(char *str, char delim)
{
	size_t n = 0, i = 0;

	while (str[i] == delim)
		++i;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] == delim && is_in_str(&str[i], '\'') % 2 == 0)
		{
			while (str[i] == delim)
				++i;
			if (str[i])
				++n;
		}
		else
			++i;
	}
	return (n + 1);
}

/**
* get_items_len - get the length of a linked list of type Item
* @items: the linked list
* Return: the length
*/
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

/**
* _substr - sub string
* @str: the string
* @bytes: number of bytes to take from string
* Return: A pointer to the new string stored in heap
*/
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

/**
* int2str - int to string
* @line_number: the int
* Return: the string
*/
char *int2str(int line_number)
{
	char *str = NULL, *str_temp = NULL;
	int temp = 0, len = 0;

	if (!line_number)
		return (_strdup("0"));

	temp = line_number;
	while (temp)
	{
		temp /= 10;
		++len;
	}
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	temp = line_number;
	str[len] = '\0';
	while (temp)
	{
		str[len - 1] = (temp % 10) + '0';
		--len;
		temp /= 10;
	}
	if (line_number < 0)
	{
		str_temp = _strdup("-");
		_str_concat(&str_temp, str);
		free(str);
		str = str_temp;
	}
	return (str);
}

/**
* uint2str - unsigned int to string
* @line_number: the unsigned int
* Return: the string
*/
char *uint2str(unsigned int line_number)
{
	char *str = NULL;
	unsigned int temp = 0, len = 0;

	if (!line_number)
		return (_strdup("0"));

	temp = line_number;
	while (temp)
	{
		temp /= 10;
		++len;
	}
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	temp = line_number;
	str[len] = '\0';
	while (temp)
	{
		str[len - 1] = (temp % 10) + '0';
		--len;
		temp /= 10;
	}

	return (str);
}


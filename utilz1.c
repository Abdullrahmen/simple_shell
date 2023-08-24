#include "main.h"

/**
* is_in_str - is delim in str
* @str: string
* @delim: The delim
* Return: number of delim in string
*/
size_t is_in_str(char *str, char delim)
{
	size_t n = 0, i = 0;

	while (str[i])
	{
		if (str[i] == delim)
			++n;
		++i;
	}
	return (n);
}

/**
* _str_concat - string concat
* @dest: dest (self change - pass by reference)
* @src: src
* Return: A pointer to dest
*/
char *_str_concat(char **dest, char *src)
{
	char *temp = NULL;
	size_t i = 0, len = 0, len2 = 0, len3 = 0;

	len = _strlen(*dest);
	len2 = _strlen(src);
	len3 = len + len2;
	temp = malloc(sizeof(char) * (len3 + 1));
	if (!temp)
		return (NULL);
	while (len3)
	{
		if (i < len)
			temp[i] = (*dest)[i];
		else
			temp[i] = src[i - len];
		++i;
		--len3;
	}
	temp[i] = '\0';
	free(*dest);
	*dest = temp;
	return (temp);
}

/**
* copy_from_delim - copy string from delim to the end
* @str: string
* @delim: delim
* Return: a pointer to the new string stored in heap
*/
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

/**
* copy_till_delim - copy string till delim
* @str:string
* @delim:delim
* Return: a pointer to the new string stored in heap
*/
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
* items2str - convert Items linked list to list of strings (name=value)
* @items: the linked list
* Return: the list of strings or NULL on failure
*/
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


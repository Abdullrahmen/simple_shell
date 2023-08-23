#include "main.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
/**
 * _getlineChecker - checks the existence of the string
 * @string: the input string
 * @n: the size of the input string
 * @stream: the file handle
 * Return: the number of characters that are read.
 */
size_t _getlineChecker(char **string, size_t *n,
		__attribute__((unused)) FILE * stream)
{
	if (string == NULL || n == NULL)
		return (-1);
	if (*string == NULL)
	{
		*n = 128; /* Maximum expected line length */
		*string = malloc(*n);
		if (*string == NULL)
			return (-1);
	}
	return (0);
}
/**
 * _getline - reads a string of text
 * @string: the input string
 * @n: the size of the input string
 * @stream: the file handle
 * Return: the number of characters that are read.
 */
size_t _getline(char **string, size_t *n, FILE *stream)
{
	size_t nchars, size;
	char *buff, *temp;

	_getlineChecker(string, n, stream);
	temp = *string;
	size = *n;
	while (1)
	{
		nchars = read(STDIN_FILENO, temp, size);
		if (nchars == ULLONG_MAX)
			return (ULLONG_MAX);
		else if (nchars == 0)
			break;
		temp += nchars;
		size -= nchars;
		if (size == 0)
		{
			size = *n * 2;
			buff = malloc(sizeof(char) * size);
			if (buff == NULL)
				return (-1);
			*string = buff;
			temp = *string + (*n - size);
			*n = size;
		}
		if (*(temp - 1) == '\n')
			break;
	}
	if (temp == *string)
		return (-1);
	*temp = '\0';
	return (temp - *string);
}

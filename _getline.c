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
	char *buffer = *string;
	size_t buffsize = *n;
	getline(&buffer, &buffsize, stream);
	return (0);
}

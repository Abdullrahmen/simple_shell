#include "main.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
/**
 * _strspn - get length of a prefix substr
 * @str: str s to process
 * @target: target search characters
 * Return: num of bytes from the init segment of str that contain accept
 */
unsigned int _strspn(char *str, char *target)
{
	unsigned int i = 0, r = 0;

	for (i = 0; str[i]; i++)
	{
		for (r = 0; target[r]; r++)
		{
			if (str[i] == target[r])
				break;
		}
		if (!target[r])
			break;
	}
	return (i);
}

/**
 * _strpbrk - searches a str for any of a set of bytes
 * @str: str s to search through
 * @target: set of bytes to search with
 * Return: p to the byte in s that matches one of the bytes in accept;
 * will return NULL if nothing is found
 */
char *_strpbrk(char *str, char *target)
{
	unsigned int i = 0, r = 0;

	for (i = 0; str[i]; i++)
	{
		for (r = 0; target[r]; r++)
		{
			if (str[i] == target[r])
				return (str + i);
		}
	}
	return (NULL);
}
/**
 * _strtok - takes in a str and splits it into tokens based on delim
 * @str: input char * string
 * @delim: str of delimiters to split up the input str
 * Return: p to the token or NULL if at the end
 */
char *_strtok(char *str, const char *delim)
{
	return (strtok(str, delim));
}

#include "main.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
/**
 * _strlen - returns the length of a string.
 * @s: char pointer
 * Return: the length *s
 */
size_t _strlen(char *s)
{
	size_t len;

	len = 0;
	if (s[0] == '\0')
		return (0);

	while (s[len] != '\0')
	{
		len++;
	}
	return (len);
}

/**
 * _strcmp - compares too strings
 * @s1: first string
 * @s2: sec string
 * Return: 0
 */
int _strcmp(char *s1, char *s2)
{
	int i;

	for (i = 0; ((s1[i] != '\0') || (s2[i] != '\0')); i++)
	{
		if (s1[i] - s2[i] != 0)
			return (s1[i] - s2[i]);
	}
	return (0);
}

/**
 * _strdup - creates an array of chars
 * @str: array size
 * Return: array refrance or NULL
 */
char *_strdup(char *str)
{
	int len;
	char *arr;

	if (str == NULL)
		return (NULL);
	len = _strlen(str);
	arr = malloc((sizeof(char) * len) + 1);
	if (arr == NULL)
		return (NULL);
	arr[len] = '\0';
	while (len--)
		arr[len] = str[len];
	return (arr);
}

/**
 * _strcpy - copies the string pointed
 * @dest: char pointer
 * @src: char pointer
 * Return: char pinter
 */
char *_strcpy(char *dest, char *src)
{
	int i;

	for (i = 0; src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
	dest[i] = '\0';
	return (dest);
}

/**
 * _strcat - concatenates two strings
 * @dest: char pointer left side
 * @src: char pointer right side
 * Return: the length *s
 */
char *_strcat(char *dest, char *src)
{
	int len, i;

	len = _strlen(dest);
	for (i = 0; src[i] != '\0'; i++)
	{
		dest[len + i] = src[i];
	}
	dest[len + i] = '\0';
	return (dest);
}

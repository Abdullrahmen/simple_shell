#include "main.h"
/**
 * _atoi - converts a string to an integer
 * @str: the required string
 * Return: on success the integer value.
 */
int _atoi(char *str)
{
	int num;
	unsigned int len, i;

	len = _strlen(str);
	num = 0;
	for (i = 0; i < len; i++)
	{
		if (str[len - (i + 1)] >= '0' && str[len - (i + 1)] <= '9')
		num = num * 10 + str[i] - '0';
	}
	return (num);
}

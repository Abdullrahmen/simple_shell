#include "main.h"
/**
 * _atoi - converts a string to an integer
 * @str: the required string
 * Return: on success the integer value.
 */
int _atoi_exit(Item *env)
{
	if (!_strcmp(get_item_value(env, EXIT_STATUS), "-1"))
	{
		if (!_strcmp(get_item_value(env, LAST_EXIT_STATUS), "-1"))
			return (0);
		else
			return (_atoi(get_item_value(env, LAST_EXIT_STATUS)));
	}
	return (_atoi(get_item_value(env, EXIT_STATUS)));
}

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

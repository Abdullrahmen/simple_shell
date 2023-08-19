#include "main.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
/**
 * get_item_value - gets the value of the name.
 * @name: the name that required to get its value.
 * Return: the value.
 */
char *get_item_value(char *name)
{
	int i = 0, j = 0;

	if (name == NULL)
		return (NULL);
	while (environ[i])
	{
		j = 0;
		while (environ[i][j] != '=')
		{
			if (environ[i][j] != name[j])
				break;
			if (environ[i][j] == name[j] && (environ[i][j + 1] == '='))
				return (&environ[i][strlen(name) + 1]);
			j++;
		}
		i++;
	}
	return (NULL);
}


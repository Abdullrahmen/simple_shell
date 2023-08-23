#include "main.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/**
 * get_item_value - gets the value of the name.
 * @items: A list of items
 * @name: the name that required to get its value.
 * Return: the value of the variable
 */
char *get_item_value(Item *items, char *name)
{
	Item *items_iter = NULL;

	if (name == EXIT_STATUS || name == LAST_EXIT_STATUS)
		return ("0");
	items_iter = items;
	if (name == NULL)
		return (NULL);
	while (items_iter)
	{
		if (_strcmp(items_iter->name, name) == 0)
			return (items_iter->value);
		else if (_strcmp(items_iter->name, name) != 0)
			items_iter = items_iter->next;
	}
	return (NULL);
}

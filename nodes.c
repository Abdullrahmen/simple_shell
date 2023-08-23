#include "main.h"

/**
 * delete_nodeint_at_index - deletes the node at index
 * @head: head of the list
 * @index: emplacement of the node to delete
 * Return: one on success and 0 on failure
 */
int delete_nodeint_at_index(Item **head, unsigned int index)
{
	Item *current, *previous;
	unsigned int counter = 0;

	if (!head || !*head)
		return (0);

	current = *head;
	if (index == 0)
	{
		*head = current->next;
		free(current->name);
		free(current->value);
		free(current);
		return (1);
	}
	while (current)
	{
		if (counter == index)
		{
			previous->next = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return (1);
		}
		++counter;
		previous = current;
		current = current->next;
	}
	return (0);
}

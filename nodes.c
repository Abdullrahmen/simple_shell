#include "main.h"

/**
 * delete_nodeint_at_index - deletes the node at index
 * @head: head of the list
 * @index: emplacement of the node to delet
 * Return: pointr to head of list
 */
int delete_nodeint_at_index(Item **head, unsigned int index)
{
	Item *current, *previous;
	unsigned int counter = 0;

	if (!head || !*head)
		return (-1);
	if (index == 0)
	{
		current = *head;
		*head = (*head)->next;
		free(current->name);
		free(current->value);
		free(current);
		return (1);
	}
	current = *head;
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
		counter++;
		previous = current;
		current = current->next;
	}
	return (-1);
}

#include "main.h"

/**
 * add_node - adds a new node at the beginning of a list_t list.
 * @head: the head of the list
 * @name: the name of the node
 * @value: the value of the node
 * Return: the address of the new element
 */
Item *add_node(Item **head, char *name, char *value)
{
	Item *new_head = malloc(sizeof(Item));

	if (!head || !new_head)
	{
		free(new_head);
		return (NULL);
	}
	new_head->name = _strdup(name);
	new_head->value = _strdup(value);
	if (!new_head->name  || !new_head->value)
	{
		free(new_head->name);
		free(new_head->value);
		free(new_head);
		return (NULL);
	}
	new_head->next = *head;
	*head = new_head;
	return (new_head);
}

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

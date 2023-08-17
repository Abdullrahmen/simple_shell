#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#define COMMAND_BUILT_IN 1
#define COMMAND_PATH 2

/* Error ids */
#define E_COMMAND 1
#define E_PATH 2
#define E_ACCESS 3


/**
* item - General item for env and alias
* @name: name
* @value: value
*/
typedef struct Item
{
	char *name;
	char *value;
} Item;


/*utilz.c -> S*/
char *_strtok(char *str, const char *delim);
size_t _getline(char **string, size_t *n, FILE *stream);
/* get one token which is the command 
* ex. ls and return the path of ls command (usr/bin/ls or built_ins/ls) */
char *get_built_in_path(char *command);

/*handle_commands.c -> A*/
int handle_commands(char *commands, Item *env, Item *alias, char *program_name);
char **filter_commands(char *commands); /*Return: list of commands*/
int handle_command(char **command, Item *env, Item *alias);
int get_command_type(char *first_token);
void handle_error(char *first_token, int error_id, char *program_name);

/*executers.c -> S*/
/* - fork
*  - access */
int command_executer(char *path, int command_type, int argc, char **argv);

/*items1.c -> */
void name2value(char **str, Item *items);
void free_item(Item *item);
void free_items_list(Item *items);
size_t get_items_len(Item *items);

/*items2.c -> */
Item *init_env(char **_env);
Item *init_alias();
char *get_item_value(char *key);
void add_item(char *key, char *value);
void set_item(char *key, char *value);
void unset_item(char *key);

#endif /*MAIN_H*/


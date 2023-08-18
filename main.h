#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>

#define COMMAND_BUILT_IN 1
#define COMMAND_PATH 2
#define MAX_LINE_LENGTH 512
#define INIT_BUFFER_SIZE 5

/* Error ids (all are must be negative) */
#define E_COMMAND_UNKNOWN -1
#define E_PATH_NOT_EXIST -2
#define E_ACCESS_FILE -3
#define E_FILE_RETURN_E -4 /* executed file return error (-1) */

/**
* item - General item for env and alias
* @name: name
* @value: value
*/
typedef struct Item
{
	char *name;
	char *value;
	char *next; /*linked list*/
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
int handle_separators(int prev_result, char *separator); /*tells if the next command should be executed or not*/
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
Item *init_env(char **_env); /*the pointer must has a value even if it points to empty Item*/ 
Item *init_alias(); /*the pointer must has a value even if it points to empty Item*/ 
char *get_item_value(char *key);
void set_item(char *key, char *value); /*if the item is already exist then will change its value*/
void unset_item(char *key);

#endif /*MAIN_H*/


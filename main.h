#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>

#define COMMAND_BUILT_IN 1
#define COMMAND_PATH 2
#define COMMAND_OUR_BUILT_IN 3
#define INIT_LINE_LENGTH 128

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
	struct Item *next; /*linked list*/
} Item;

/*utilz.c -> Samy*/
/*--------------Helpers--------------*/
char *_strtok(char *str, const char *delim);
size_t _getline(char **string, size_t *n, FILE *stream);
char *get_built_in_path(char *first_token, Item *env);
char *_strcat(char *dest, char *src);
char *_strcpy(char *dest, char *src);
size_t _strlen(char *s);
int _strcmp(char *s1, char *s2);
char *_strdup(char *str);

/*handle_commands.c -> A*/
int handle_commands(char *commands, Item *env, Item *alias, char *program_name);
Item *filter_commands(char *commands, char *program_name); /*Return: list of commands*/
int handle_command(char *command, Item *env, Item *alias); /*one command only*/
int handle_separators(int prev_result, char separator); /*tells if the next command should be executed or not*/
void handle_error(char *command, int error_id, char *program_name);
int get_command_type(char *first_token, Item *env);
char *_substr(char *str, size_t bytes);
size_t get_n_tokens(char *str, char delim);
/*executers.c -> S*/
/* - fork
*  - access */
int command_executer(char *path, int command_type, int argc, char **argv);

/*items_init_free.c -> A*/
Item *init_env(char **_env); /*the pointer must has a value even if it points to an empty Item*/ 
Item *init_alias(); /*the pointer must has a value even if it points to an empty Item*/ 
void free_item(Item *item);
void free_items_list(Item *items);
void name2value(char **str, Item *alias, Item *env);

/*------------items_utilize-------------*/
size_t get_items_len(Item *items); /*if it's only one empty item then will return 0*/
char *get_item_value(Item *items, char *name);
void set_item(Item *items, char *name, char *value); /*if the item is already exist then will change its value*/
void unset_item(Item *items, char *name);

/* built_in_functions.c -> Samy*/
int _exit_(int status);
int _env_(Item *env);
int _setenv_(Item *env, char *name, char *value);
int _unsetenv_(Item *env, char *name);
int _cd_(Item *env, char *directory, char *program_name);

#endif /*MAIN_H*/


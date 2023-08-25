#ifndef MAIN_H
#define MAIN_H
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/wait.h>
#include <string.h>
#define INIT_LINE_LENGTH 128
#define EXIT_STATUS "___EXIT_STATUS___"
#define LAST_EXIT_STATUS "___LAST_EXIT_STATUS___"
/*----Errors IDs (all are must be negative)----*/
#define E_COMMAND_UNKNOWN -1
#define E_PATH_NOT_EXIST -2
#define E_PERMISSION_DENIED -3
#define E_FILE_RETURN_E -4 /* executed file return error (-1) */
#define E_ILLEGAL_EXIT_NUMBER -5
#define E_DIRECTORY_UNFOUND -6
#define E_INVALID_ENV_NAME -7
#define E_INVALID_ARGUMENTS -8

/**
* struct Item - General item
* @name: name
* @value: value
* @next: next
*/
typedef struct Item
{
	char *name;
	char *value;
	struct Item *next; /*linked list*/
} Item;

/*------------------------handle_commands_1-----------------------*/
char **init_argv(char *command);
int handle_commands(char *commands, Item **env, Item **alias,
		char *program_name, unsigned int line_number);
Item *filter_commands(char *commands, char *program_name,
		unsigned int line_numbet); /*Return: list of commands*/
int handle_command(char *command, Item **env, Item **alias,
	char *program_name, unsigned int line_number); /*one command only*/

/*------------------------handle_commands_2------------------------*/
int check_command_type(char *first_token, Item *env, char **path);
int handle_our_built_in(char **argv, Item **env, Item **alias, int *is_exit);

/*------------------------handle_commands_3-----------------------*/
void handle_errors(char **argv, int error_id, char *program_name,
		unsigned int line_number, Item **env);
int handle_separators(int prev_result, char separator);
int command_executer(char *path, char **argv, Item **env);

/*------------------utilz1------------------*/
char *_str_concat(char **dest, char *src);
size_t is_in_str(char *str, char delim);
char **items2str(Item *items);
char *copy_from_delim(char *str, char delim);
char *copy_till_delim(char *str, char delim);

/*------------------utilz2----------------*/
char *_substr(char *str, size_t bytes);
size_t get_n_tokens(char *str, char delim);
size_t get_items_len(Item *items);
char *int2str(int line_number);
char *uint2str(unsigned int line_number);

/*----------------items_init_free------------------*/
Item *init_env(char **_env);
void free_items_list(Item *items);
int _setenv_(Item **env, char *name, char *value);
Item *add_node(Item *head, char *name, char *value);
char *skip_spaces(char *str);

/*----------------------alias*---------------------*/
int _alias_(Item **alias, Item **env, char **argv);
Item *init_alias();

/*------------------items_utilize------------------*/
void name2value(char **str, Item *env, Item *alias);
char *get_item_value(Item *items, char *name);

/*-------------------strings-------------------*/
char *_strtok(char *str, const char *delim);
char *_strcat(char *dest, char *src);
char *_strcpy(char *dest, char *src);
size_t _strlen(char *s);
int _strcmp(char *s1, char *s2);
char *_strdup(char *str);

/*-------------------atoi-------------------*/
int _atoi(char *str);
int _atoi_exit(Item *env);

/*-------------------------nodes---------------------------*/
int delete_nodeint_at_index(Item **head, unsigned int index);

/*------------------items_utilize-------------------*/
void set_item(Item *items, char *name, char *value);
void unset_item(Item *items, char *name);

/*------------------built_in_functions------------------*/
int _env_(Item *env);
int _unsetenv_(Item **env, char *name);
int _cd_(Item *env, char *directory);
size_t _getline_2(char **string, size_t *n, FILE *stream);

#endif /*MAIN_H*/


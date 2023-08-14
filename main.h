#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#define COMMAND_BUILT_IN 1
#define COMMAND_PATH 2
#define EXIT 0
#define EXIT_CODE 0

/*key value*/
extern char **g_env;

/*utilz.c -> S*/
char *_strtok(char *str, const char *delim);
size_t _getline(char **string, size_t *n, FILE *stream);

/*handle_commands.c -> A*/
void handle_commands(char *str);
void handle_command(char *str);
int get_command_type(char *first_token);
char **filter_command(char *str); /*Return: list of commands*/

/*executers.c -> S*/
/* get one token which is the command ex. ls and return the path of ls command (usr/bin/ls or built_ins/ls) */
char *get_built_in_path(char *command);
/* - $env_var
*  - fork
*  - access */
int command_executer(char *path, int command_type, int argc, char **argv);

/*env_handles.c -> S*/
char *get_env_value(char *key);
void add_env_var(char *key, char *value);
void set_env_var(char *key, char *value);
void unset_env_var(char *key);

#endif /*MAIN_H*/


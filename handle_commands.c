#include "main.h"

/**
* handle_separators - handle separators (tell if the next command should be 
* executed or not)
* @prev_result: previous command result (-number is error, 1 is passed,
* prev_result shouldn't have the value zero but if it is then will return 0)
* @separator: the separator between the commands ex. && or || or ;
* Return: one if the next command should be executed and zero otherwise
*/
int handle_separators(int prev_result, char *separator)
{}

/**
* handle_command - handle one command only
* @command: the command
* @env: the environment list
* @alias: the alias list
* Return: zero if the command tell the shell to exit (exit interactive loop)
* and negative if there is an error (look at error ids)
* and one otherwise
*/
int handle_command(char **command, Item *env, Item *alias)
{}

/**
* filter_commands - filter the commands from comments and separators
* @commands: one line of commands
* @Return: list of commands separated by separators
* ex. ls;ls||ls -> [ls, ;, ls, ||, ls]
*/
char **filter_commands(char *commands)
{}


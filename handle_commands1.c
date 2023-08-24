#include "main.h"
/**
* init_argv - init argv
* @command: the cleaned and formatted command
* Return: argv
*/
char **init_argv(char *command)
{
	char **argv = NULL, *temp = NULL;
	size_t i = 0;

	argv = malloc(sizeof(*argv) * (get_n_tokens(command, ' ') + 1));
	if (!argv)
		return (NULL);

	argv[0] = _strtok(command, " ");
	while (argv[i])
	{
		if (is_in_str(argv[i], '\'') % 2 == 1)
		{
			temp = _strtok(NULL, " ");
			if (!temp)
			{
				argv[i + 1] = NULL;
				break;
			}
			--temp;
			*temp = ' ';
		}
		++i;
		argv[i] = _strtok(NULL, " ");
	}
	return (argv);
}

/**
* handle_command - handle one command only
* @command: the command
* @env: the environment list
* @alias: the alias list
* @program_name: argv[0]
* @line_number: line number (used in errors)
* Return: zero if the command tell the shell to exit (exit interactive loop)
* and one otherwise
*/
int handle_command(char *command, Item **env, Item **alias, char *program_name,
			unsigned int line_number)
{
	char **argv = NULL, *path = NULL;
	int command_type = 0, command_result = 0, is_exit = 0, error_id = 0;

	if (!command || !*command)
		return (1);
	argv = init_argv(command);

	command_type = handle_our_built_in(argv, env, alias, &is_exit);
	if (!command_type)
	{
		command_type = check_command_type(argv[0], *env, &path);
		if (command_type >= 0)
			command_result = command_executer(path, argv, env);
		free(path);
	}
	if (command_type < 0)
		error_id = command_type;
	if (command_result)
		error_id = E_FILE_RETURN_E;
	if (is_exit)
	{
		free(argv);
		return (0);
	}
	handle_errors(argv, error_id, program_name, line_number, env);
	free(argv);
	if (error_id < 0)
		return (error_id);
	return (1);
}

/**
* handle_commands - take a line of commands and handle them
* @commands: the line of commands
* @env: the environment list
* @alias: the alias list
* @program_name: the program name (argv[0])
* @line_number: line number
* Return: zero if the commands tell the shell to exit (exit interactive loop)
* and one otherwise
*/
int handle_commands(char *commands, Item **env, Item **alias,
	char *program_name, unsigned int line_number)
{
	Item *commands_list = NULL, *iter_command;
	int command_result = 0, exec_next_command = 1;

	commands_list = filter_commands(commands, program_name, line_number);
	if (!commands_list)
		return (1);
	iter_command = commands_list;
	while (iter_command && exec_next_command)
	{
		if (!*iter_command->value)
			break;
		name2value(&iter_command->value, *env, *alias);
		command_result = handle_command(iter_command->value, env,
					alias, program_name, line_number);
		if (!command_result)
		{
			free_items_list(commands_list);
			return (0);
		}
		exec_next_command = handle_separators(command_result, *iter_command->name);
		iter_command = iter_command->next;
	}
	free_items_list(commands_list);
	return (1);
}


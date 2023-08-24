#include "main.h"

/**
* errors_msgs - errors messages
* @argv:argv
* @error_id:error id
* @error_msg: error message
* @env: environment
*/
void errors_msgs(char **argv, int error_id, char **error_msg, Item **env)
{
	switch (error_id)
	{
	case (E_COMMAND_UNKNOWN):
	case (E_PATH_NOT_EXIST):
	case (E_INVALID_ARGUMENTS):
		_str_concat(error_msg, argv[0]);
		_str_concat(error_msg, ": not found");
		_setenv_(env, LAST_EXIT_STATUS, "127");
		break;
	case (E_PERMISSION_DENIED):
		_str_concat(error_msg, argv[0]);
		_str_concat(error_msg, ": Permission denied");
		_setenv_(env, LAST_EXIT_STATUS, "127");
		break;
	case (E_ILLEGAL_EXIT_NUMBER):
		_str_concat(error_msg, argv[0]);
		_str_concat(error_msg, ": Illegal number: ");
		_str_concat(error_msg, argv[1]);
		_setenv_(env, LAST_EXIT_STATUS, "2");
		break;
	case (E_DIRECTORY_UNFOUND):
		_str_concat(error_msg, argv[0]);
		_str_concat(error_msg, ": can't ");
		_str_concat(error_msg, argv[0]);
		_str_concat(error_msg, " to ");
		_str_concat(error_msg, argv[1]);
		_setenv_(env, LAST_EXIT_STATUS, "2");
		break;
	}
}

/**
* handle_errors - handle errors according to error id
* @argv: argv
* @error_id: error id
* @program_name: program(shell) name
* @line_number: command line number
* @env: env
*/
void handle_errors(char **argv, int error_id, char *program_name,
				unsigned int line_number, Item **env)
{
	char *error_msg = NULL, *str = NULL;

	if (!argv || !*argv || !**argv || error_id >= 0 || !line_number)
		return;
	if (error_id == E_FILE_RETURN_E)
		return;

	error_msg = _strdup(program_name);
	_str_concat(&error_msg, ": ");
	str = uint2str(line_number);
	_str_concat(&error_msg, str);
	_str_concat(&error_msg, ": ");
	free(str);

	errors_msgs(argv, error_id, &error_msg, env);

	_str_concat(&error_msg, "\n");
	write(STDERR_FILENO, error_msg, _strlen(error_msg));
	free(error_msg);
}

/**
* handle_separators - handle separators (tell if the next command should be
* executed or not)
* @prev_result: previous command result (-number is error, 1 is passed,
* prev_result shouldn't have the value zero but if it is then will return 0)
* @separator: the separator between the commands ex. && or || or ;
* Return: one if the next command should be executed and zero otherwise
*/
int handle_separators(int prev_result, char separator)
{
	if (separator == ' ')
		return (0);
	if (separator == '#')
		return (0);

	if (prev_result <= 0 && separator == '&')
		return (0);
	if (prev_result == 1 && separator == '|')
		return (0);
	return (1);
}

/**
* command_executer - command executer
* @path: the path to the executable file
* @argv: argv
* @env: environment
* Return: Status of the file that had been executed
*/
int command_executer(char *path, char **argv, Item **env)
{
	char *tmp = NULL, **_env = NULL;
	size_t i = 0;
	int pid = 0;
	int status = 0;

	_env = items2str(*env);
	pid = fork();
	if (!pid)
	{
		execve(path, argv, _env);
		i = 0;
		while (_env[i])
			free(_env[i++]);
		free(_env);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	tmp = int2str(WEXITSTATUS(status));
	_setenv_(env, LAST_EXIT_STATUS, tmp);
	free(tmp);
	i = 0;
	while (_env[i])
		free(_env[i++]);
	free(_env);

	if (status)
		return (E_FILE_RETURN_E);
	return (0);
}



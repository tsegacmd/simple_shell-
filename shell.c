#include "holberton.h"

/**
 * main - main loop of shell
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	char **tokens = NULL;
	char **environ_copy = NULL;
	char *full_path;
	char *path;
	int status;
	int execve_status;
	int builtin_status;
	struct stat buf;
	pid_t child_pid;
	unsigned int length;
	unsigned int i;

	path = _getenv("PATH", environ_copy);

	while (TRUE)
	{
		/* check interactive / non-interactive mode */
		prompt(STDIN_FILENO, buf);

		/* get input from user */
		line = _getline(stdin, line);
		if (line == NULL)
			return (0);
		if (_strcmp(line, "\n", 1) == 0)
			continue;
		/* remove newline character from input */
		length = _strlen(line);
		/*if (_strcmp(line, "\n", length) == 0)
			continue; */
		/*line = parser(line); */
		length = _strlen(line);

		line[length - 1] = '\0';

		/* tokenize input */
		tokens = malloc(sizeof(char) * BUFFER);
		if (tokens == NULL)
		        continue;

		tokens = _strtok(line, tokens);

		/* check for builtins */
		builtin_status = builtin_execute(tokens);
		if (builtin_status == 0)
			exit(EXIT_SUCCESS);

		/* check PATH for executables */
		full_path = _which(tokens[0], full_path, path);
		if (full_path == NULL)
			full_path = tokens[0];

		/* create child process to run executables */
		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Error");
			exit(EXIT_FAILURE);
		}
		if (child_pid == 0)
		{
			execve_status = execve(full_path, tokens, NULL);
			if (execve_status == -1)
			{
				perror("Error");
				exit(EXIT_FAILURE);
			}
		}
		else
			wait(&status);

		for (i = 0; tokens[i] != '\0'; i++)
			free(tokens[i]);
		free(tokens);

		free(full_path);
	}
	return (0);
}

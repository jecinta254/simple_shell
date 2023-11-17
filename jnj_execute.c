#include "shell.h"
/**
 * jnj_execute - Execute a command.
 * @watho: The command to execute.
 */
void jnj_execute(const char *watho)
{
char **argument_s;
char *token;
int status;
int arg_ument_count = 0;
char *delm = " ";
pid_t mtoto_pid;
token = strtok((char *)watho, delm);
argument_s = malloc(sizeof(char *) * 2);

while (token)
{
argument_s[arg_ument_count] = strdup(token);
token = strtok(NULL, delm);
arg_ument_count++;
argument_s = realloc(argument_s, sizeof(char *) * (arg_ument_count + 1));
}
argument_s[arg_ument_count] = NULL;
mtoto_pid = fork();

if (mtoto_pid == -1)
{
perror("Fork failed");
exit(EXIT_FAILURE); }
else if (mtoto_pid == 0)
{
if (execvp(argument_s[0], argument_s) == -1)
{
perror("Error in execvp");
exit(EXIT_FAILURE); }
exit(EXIT_SUCCESS); }
else
{
waitpid(mtoto_pid, &status, 0);
}
free(argument_s);
}

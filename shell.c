#include "shell.h"

/**
* main - the entry point of the shell program.
* Return: Always 0.
*/

int main(void)
{

char watho[BUFFER_SIZE];

if (isatty(fileno(stdin)))
{
while (true)
{
jnj_prompt();
jnj_readcmd(watho, sizeof(watho));

if (feof(stdin))
{
jnj_print("\n");
break;
}
if (watho[0] != '\0' && watho[0] != '\n')
{
jnj_execute(watho);
}
}
}
else
{
while (fgets(watho, sizeof(watho), stdin) != NULL)
{
watho[strcspn(watho, "\n")] = '\0';
if (watho[0] != '\0')
{
jnj_execute(watho);
}
}
}
return (0);

}


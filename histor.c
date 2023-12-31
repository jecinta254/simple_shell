#include "shell.h"
/**
 * our_get_history_file - gets the history file
 * @info: parameter struct
 * Return: allocated string containg history file
 */
char *our_get_history_file(our_info_t *info)
{
	char *our_buf, *our_dir;

	our_dir = getEnvironments(info, "HOME=");
	if (!our_dir)
		return (NULL);
	our_buf = malloc(sizeof(char) *
	(our_strlen(our_dir) + our_strlen(OUR_HIST_FILE) + 2));
	if (!our_buf)
		return (NULL);
	our_buf[0] = 0;
	our_strcpy(our_buf, our_dir);
	our_strcat(our_buf, "/");
	our_strcat(our_buf, OUR_HIST_FILE);
	return (our_buf);
}

/**
 * our_write_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 * Return: 1 on success, else -1
 */
int our_write_history(our_info_t *info)
{
	ssize_t f;
	char *filename = our_get_history_file(info);
	our_list_t *node = NULL;

	if (!filename)

		return (-1);
	f = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (f == -1)
		return (-1);
	for (node = info->our_history; node; node = node->next)
	{
		_our_putsfd(node->str, f);
		_our_putfd('\n', f);
	}
	_our_putfd(OUR_BUF_FLUSH, f);
	close(f);
	return (1);
}

/**
 * our_read_history - reads history from file
 * @info: the parameter struc
 * Return: histcount on success, 0 otherwise
 */
int our_read_history(our_info_t *info)
{
	int i, last = 0, count = 0;
	ssize_t f, rlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = our_get_history_file(info);

	if (!filename)
		return (0);
	f = open(filename, O_RDONLY);
	free(filename);
	if (f == -1)
		return (0);
	if (!fstat(f, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rlen = read(f, buf, fsize);
	buf[fsize] = 0;
	if (rlen <= 0)
		return (free(buf), 0);
	close(f);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			our_build_history_list(info, buf + last, count++);
			last = i + 1;
		}
	if (last != i)
		our_build_history_list(info, buf + last, count++);
	free(buf);
	info->our_histcount = count;
	while (info->our_histcount-- >= OUR_HIST_MAX)
		our_delete_node_at_index(&(info->our_history), 0);
	our_renumber_history(info);
	return (info->our_histcount);
}

/**
 * our_build_history_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @our_buf: buffer
 * @count: the history linecount, histcount
 * Return: Always 0
 */
int our_build_history_list(our_info_t *info,
char *our_buf, int count)
{
	our_list_t *node = NULL;

	if (info->our_history)
		node = info->our_history;
	our_add_node_end(&node, our_buf, count);
	if (!info->our_history)
		info->our_history = node;
	return (0);
}

/**
 * our_renumber_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maint
 * Return: the new histcount
 */
int our_renumber_history(our_info_t *info)
{
	our_list_t *node = info->our_history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->our_histcount = i);
}

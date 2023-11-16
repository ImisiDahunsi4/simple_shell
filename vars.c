#include "shell.h"
/**
 * replace_string - Replaces string
 * @old: Address of the old string
 * @new: New string
 *
 * Returns: 1 if replaced, 0 otherwise
 */
int replace_string(char **old, char *new)
{
        free(*old);
        *old = new;
        return 1;
}


/**
 * check_chain - Checks if we should continue chaining based on the last status
 * @info: Parameter structure
 * @buf: Character buffer
 * @p: Address of the current position in buf
 * @i: Starting position in buf
 * @len: Length of buf
 *
 * Returns: Void
 */
void check_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->cmd_buf_type == CMD_AND) {
		if (info->status) {
			buf[i] = '\0';
			j = len;
		}
	}
	if (info->cmd_buf_type == CMD_OR) {
		if (!info->status) {
			buf[i] = '\0';
			j = len;
		}
	}
	*p = j;
}

/**
 * replace_alias - Replaces an alias in the tokenized string
 * @info: Parameter structure
 *
 * Returns: 1 if replaced, 0 otherwise
 */
int replace_alias(info_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++) {
		node = node_starts_with(info->alias, info->argv[0], '=');
		if (!node)
			return 0;
		free(info->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return 0;
		p = _strdup(p + 1);
		if (!p)
			return 0;
		info->argv[0] = p;
	}
	return 1;
}

/**
 * replace_vars - Replaces variables in the tokenized string
 * @info: Parameter structure
 *
 * Returns: 1 if replaced, 0 otherwise
 */
int replace_vars(info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->argv[i]; i++) {
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!_strcmp(info->argv[i], "$?")) {
			replace_string(&(info->argv[i]),
				_strdup(convert_number(info->status, 10, 0)));
			continue;
		}
		// Other checks and replacements...
	}
	return 0;
}


/**
 * is_chain - Checks whether the current character in the buffer is a chain delimiter
 * @info: Parameter structure
 * @buf: Character buffer
 * @p: Address of the current position in buf
 *
 * Returns: 1 if it's a chain delimiter, 0 otherwise
 */
int is_chain(info_t *info, char *buf, size_t *p)
{
	size_t j = *p;

	switch (buf[j]) {
	case '|':
		if (buf[j + 1] == '|') {
			buf[j] = '\0';
			j++;
			info->cmd_buf_type = CMD_OR;
			break;
		}
		return 0;

	case '&':
		if (buf[j + 1] == '&') {
			buf[j] = '\0';
			j++;
			info->cmd_buf_type = CMD_AND;
			break;
		}
		return 0;

	case ';':
		buf[j] = '\0';
		info->cmd_buf_type = CMD_CHAIN;
		break;

	default:
		return 0;
	}
	*p = j;
	return 1;
}


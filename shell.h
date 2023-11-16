
#ifndef _SHELL_H_
#define _SHELL_H_


#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

/* Flags for convert_number() */
#define CONVERT_LOWERCASE   1
#define CONVERT_UNSIGNED    2
/* Read/Write buffer sizes */
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1
#define READ_BUF_SIZE 1024


/* Command chaining types */
#define CMD_NORM    0
#define CMD_AND     2
#define CMD_CHAIN   3
#define CMD_OR      1

/* Define history file and maximum history size */
#define HIST_FILE   ".simple_shell_history"
#define HIST_MAX    4096

/* Externally declared environment variables */
extern char **environ;

/*
 * struct liststr - Structure defining a node in a singly linked list
 * @num: Integer field representing a number within the node
 * @str: String field storing a string value in the node
 * @next: Pointer to the next node in the linked list
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/*
 * struct passinfo - Contains various details to pass into a function, ensuring a consistent structure for function pointer calls.
 * @arg: String formed from getline that holds arguments
 * @argv: Array of strings derived from 'arg'
 * @path: Path string for the ongoing command
 * @argc: Count of arguments
 * @line_count: Number of encountered errors
 * @err_num: Error code for 'exit()' operations
 * @linecount_flag: Indicates whether to count this line of input
 * @fname: Name of the program's file
 * @env: Local linked list copy of the system environment variables
 * @environ: Customized modified copy of environment variables from the linked list 'env'
 * @history: Node reference for command history
 * @alias: Node reference for aliases
 * @env_changed: Flag showing modifications in 'environ'
 * @status: Status returned from the last executed command
 * @cmd_buf: Address of pointer to 'cmd_buf'; activated if chaining commands
 * @cmd_buf_type: Type of command chaining (CMD_type ||, &&, ;)
 * @readfd: File descriptor for reading line input
 * @histcount: Count of history line numbers
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	char **cmd_buf; /* Pointer to command chaining buffer */
	int cmd_buf_type; /* Type of command chaining */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT { \
    NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
    0, 0, 0 \
}

/*
 * struct builtin - Holds information about a specific built-in command and its related function
 * @type: Flag representing the built-in command
 * @func: Corresponding function associated with the built-in command
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* Function prototypes */
/* Shell execution functions */
void find_cmd(info_t *);
void fork_cmd(info_t *);
int hsh(info_t *, char **);
int find_builtin(info_t *);

/* Other utility functions */
int _eputchar(char);
int _putfd(char c, int fd);
void _eputs(char *);
int _putsfd(char *str, int fd);

/* Parsing functions */
char *dup_chars(char *, int, int);
char *find_path(info_t *, char *, char *);
int is_cmd(info_t *, char *);
/* String manipulation functions */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);
/* Exit-related functions */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);
/* Tokenization functions */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* Conversion and error functions */
int _atoi(char *);
int _erratoi(char *);
void print_error(info_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);
int interactive(info_t *);
int is_delim(char, char *);
int _isalpha(int);
/* Environment-related functions */
char *_getenv(info_t *, const char *);
char **get_environ(info_t *);
int _unsetenv(info_t *, char *);
int _setenv(info_t *, char *, char *);

/* Linked list manipulation functions */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);
/* History functions */
int replace_alias(info_t *);
int replace_vars(info_t *);
int replace_string(char **, char *);
int is_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
/* Variable-related functions */
char *get_history_file(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);
int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);
/* Built-in command functions */
int _myhistory(info_t *);
int _myalias(info_t *);
int _myenv(info_t *);
int _myexit(info_t *);
int _mycd(info_t *);
int _myhelp(info_t *);
int _mysetenv(info_t *);
int _myunsetenv(info_t *);
int populate_env_list(info_t *);
/* Memory management functions */
void *_realloc(void *, unsigned int, unsigned int);
int bfree(void **);
char *_memset(char *, char, unsigned int);
void ffree(char **);
#endif /* _SHELL_H_ */

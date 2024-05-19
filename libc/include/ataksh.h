#ifndef ATAKSH_H
#define ATAKSH_H

// TODO: Put on hold until libc is build */
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "logger.h"

/* Exit status */
#define ERROR -1
#define SUCCESS 0

/* Define inital size of commands */
#define CMDSZ 1048
/* Defines the delimeter of the passed in commands for parsing */
#define DELIMETER " "

/* Main lopp of the program: loops while parsing the client command and executing the command,
   until the client ends the session with "exit" or "q". */
int ataksh_proc();

/* Parses the passed in command counting the amount of charecters */
char **ataksh_parse_char();

/* Takes in the cmd string, and the amount of spaces in the string (bufSZ).
   Then parses the tokens of the passed in command, returning an array of strings,
   counting the amount of charecters */
char **ataksh_parse_tokens(char *cmd, int bufSZ);

/* Forks a new child process to handle the passed in command (calling exec),
   while the parent waits for the child to exit without error. */
int ataksh_fork(char **cmd);

/* TODO: Add a command history that clients can regex into */
int ataksh_history(char *args);

/* TODO: Add termios input capture for history and tab completion */

/* Gets the hostname of the machine */
char *ataksh_get_hostname();
#endif


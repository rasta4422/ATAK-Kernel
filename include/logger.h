#ifndef LOGGER_H
#define LOGGER_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

/* Define gloabl variables indicating how big the log 
   buffer is, and the different types of log levels. */
#define LOGBUFFSZ 2048
#define LOG_INFO 0
#define LOG_WARNING 1
#define LOG_ERROR 2


/* Get the current time for logging. */
char *getTime();

/* Open the Log file and parse the passed in log level.
   Write to the log file based on the level. */
void logger(const char *msg, int level);

#endif


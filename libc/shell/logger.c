#include "logger.h"

char *getTime(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char *currTime = (char *)malloc(sizeof(char)*LOGBUFFSZ);
    
    snprintf(currTime, LOGBUFFSZ, "%02d-%02d-%d %02d:%02d:%02d", 
             tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
    
    return currTime;
}

void logger(const char *msg, int level)
{
    /* TODO: Add functionality for variable args, something like printf */
    char *time = getTime();
    FILE *fp = fopen("log/ataksh.log", "a");

    if (level == LOG_ERROR)
        fprintf(fp, "%s:::ERROR:::%d:::%s\n", time, errno, msg);
    
    else if (level == LOG_WARNING)
        fprintf(fp, "%s:::WARNING:::%s\n", time, msg);

    else
        fprintf(fp, "%s:::INFO:::%s\n", time, msg);

    fflush(fp);
    fclose(fp);
    free(time);
}


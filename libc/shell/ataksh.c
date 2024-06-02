#include "ataksh.h"

/* Used to parse the command, then later free it */
char *cmd_toks = NULL;


char *ataksh_get_hostname()
{
    char *host = (char *)malloc(sizeof(char)*_SC_HOST_NAME_MAX);
    if (host == NULL){
        logger("[Host malloc]", LOG_ERROR);
    }
    if (gethostname(host, _SC_HOST_NAME_MAX)){
        logger("[Get host]", LOG_ERROR);
        return NULL;
    };
    
    return host;
}

char **ataksh_parse_char()
{
    char *tmp_cmd = (char *)malloc(CMDSZ*sizeof(char));
    memset(tmp_cmd, 0, CMDSZ*sizeof(char));
    if (tmp_cmd == NULL)
    {
        logger("[Alloction of cmd]", LOG_ERROR);
        return NULL;
    }
    
    char chr;
    int i = 0, bufSZ = 1, chrSZ = CMDSZ;
    
    while(read(STDIN_FILENO, &chr, 1)>0)
    {
        if (chr == '\n')
            break;
        else if (chr == ' ') /* count the number of spaces */
            bufSZ++;

        tmp_cmd[i++] = chr;
        if (i >= chrSZ)
        {
            chrSZ += CMDSZ;
            tmp_cmd = (char *)realloc(tmp_cmd, sizeof(char)*chrSZ);
            if (tmp_cmd == NULL){
                logger("[Reallocation of cmd]", LOG_ERROR);
                free(tmp_cmd);
                return NULL;
            }
        }
    }
    
    char *cmd = strdup(tmp_cmd);
    free(tmp_cmd);
   
    char logmsg[LOGBUFFSZ];
    snprintf(logmsg, LOGBUFFSZ, "[CMD: %s]", cmd);
    logger(logmsg, LOG_INFO);

    return ataksh_parse_tokens(cmd, bufSZ);
}

char **ataksh_parse_tokens(char *cmd, int bufSZ)
{
    char **tok_arr = (char **)malloc(sizeof(char *) * (bufSZ+1));
    memset(tok_arr, 0, sizeof(char *) * bufSZ);
    
    if (tok_arr != NULL)
    {
        int i = 0;
        cmd_toks = strdup(cmd);
        for (char *tok = strtok(cmd_toks, DELIMETER); tok != NULL; tok = strtok(NULL, DELIMETER)) 
        {
            if (strlen(tok) > 0){
                logger(tok, LOG_INFO);
                tok_arr[i++] = tok;
            }
        }
        tok_arr[i] = NULL;
        free(cmd);
        return tok_arr;
    } else 
    {
        logger("[Allocation of token array]", LOG_ERROR);
        return NULL;
    }
}

int ataksh_fork(char **cmd)
{
    pid_t pid;
    int ret;
    
    if ((pid = fork()) < 0)
        return ERROR;

    if (pid == 0)
    {
        if (execvp(cmd[0], cmd) == -1)
        {
            perror("[execvp]");
            exit(ERROR);
        }
    
    } else 
    {
        do {
            if (waitpid(pid, &ret, WUNTRACED) == ERROR)
            {
                logger("[waitpid]", LOG_ERROR);
                return ERROR;
            }
        } while(!WIFEXITED(ret) && !WIFSIGNALED(ret)); /* wait until client exits without error */
    }
    return SUCCESS;
}

int ataksh_proc(){

    char *hostname;
    if ((hostname = ataksh_get_hostname()) == NULL)
        logger("[Hostname]", LOG_ERROR);

    while (1)
    {
        printf("[%s]$ ", hostname);
        fflush(stdout);
        char **cmd_arr = ataksh_parse_char();
        
        /* TODO: Need better way to do this */
        if (strcmp(cmd_arr[0], "exit")==0 || strcmp(cmd_arr[0], "q")==0)
        {    
            free(cmd_toks);
            free(cmd_arr);
            break;
        }

        if (ataksh_fork(cmd_arr))
        {
            free(cmd_toks);
            free(cmd_arr);
            logger("[ataksh_fork failed]", LOG_ERROR);
            return ERROR;
        }
        free(cmd_toks); 
        free(cmd_arr);
    }
    return SUCCESS;
}
int main(void){
    /* TODO: configs, history, tab completion, etc. */

    if(ataksh_proc())
        return ERROR;

    return SUCCESS;
}

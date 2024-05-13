#include "atak_kernel.h"
#include "ataksh.h"
#include "logger.h"
#include "thread.h"

int shell(void *pt)
{
    if(ataksh_proc())
        return ERROR;

    return SUCCESS;
}


int main(void)
{
    char *scheduler = "SP";
    int quantum = 1000;

    if(thread_init(scheduler, quantum) != 0)
    {
        return -1;
    }
    

    int priority = 1;
    int id1 = thread_create(shell, &priority, priority);
    
    if(id1 == -1)
    {
        return -1;
    }

    int ret = thread_join(id1);
    if(ret == -1)
    {
        return -1;
    }
    return 0;
}

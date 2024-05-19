#include "thread.h"
#include "tstructs.h"

schedulerType stype = SP;
TCB *t_running = NULL;
TCB *threads[MAX_THREADS];

// keep track of thread count for getting id's
int thread_num = 0;

// Ready queues
sp_queue *ready_queue_sp;

// Other queues
struct join_queue *j_queue = NULL;
fin_queue *f_queue = NULL;
struct block_queue *b_queue = NULL;

struct itimerval timer;
struct sigaction timer_action;

// used to handle multiple signals during block, also used in DP
volatile sig_atomic_t signal_count = 0;

// used to handle quanta expirations for DP
volatile sig_atomic_t quant_count = 0;

// x variable for DP scheduler
static int dp_x;

/*
 * Function:
 *  Get min thread ID based on current thread_num
 * Returns:
 *  id of new thread on success, -1 on failure
 */
int get_thread_id()
{
    thread_num++;
    if (thread_num < MAX_THREADS-1){ 
        return thread_num;
    }
    return -1;
}

void timer_handler(int sig) 
{
    signal_count++;
    quant_count++;
    switchThreads();
    reset_timer();
}

int set_timer(int quantum)
{
    long tvSec = quantum/1000000;
    timer.it_value.tv_sec = tvSec;
    timer.it_value.tv_usec = quantum;
    timer.it_interval.tv_sec = tvSec;
    timer.it_interval.tv_usec = quantum;

    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1)
        return -1;

    return 0;
}

int reset_timer(void)
{

    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1)
        return -1;

    return 0;
}

int disable_interrupt(const sigset_t *set)
{
    if(sigprocmask(SIG_BLOCK, set, NULL) == -1)
        return -1;

    return 0;
}

int enable_interrupt(const sigset_t *set)
{
    if(sigprocmask(SIG_UNBLOCK, set, NULL) == -1)
        return -1;

    return 0;
}

void stub(void(*start_routine)(void *), void *arg)
{
    start_routine(arg);
    thread_exit();
}

int switchThreads(void)
{
    // Check quanta count against dp x
    if (quant_count == dp_x && stype == DP)
    {
        change_priority(ready_queue_sp, t_running);
        quant_count = 0;
    }

    // Make sure switchThreads only runs once
    // if multiple signals happen during block
    if (signal_count > 1)
    {
        signal_count = 0;
        reset_timer();
        return 0;
    }

    volatile int flag = 0;
    if (getcontext(&(t_running->context_)) == -1)
        return -1; 

    if (disable_interrupt(&timer_action.sa_mask) == -1)
    {
        logger("Block Interrupt", LOG_ERROR);
        return -1;
    }

    if (flag == 1)
        return 0;
    flag = 1;

    // only enqueue current thread if not finished
    if (t_running->state_ != FINISHED)
    {
        enqueue_sp(ready_queue_sp, t_running);
        t_running->state_ = READY;
    }

    // get next thread	
    t_running = dequeue_sp(ready_queue_sp);
    t_running->state_ = RUNNING;

    if (enable_interrupt(&timer_action.sa_mask) == -1)
    {
        logger("join enable inturrpt",LOG_ERROR);
        return -1;
    }

    setcontext(&(t_running->context_)); // switch context to new thread
    return 0;
}

int thread_init(char *scheduler, int quantum, ...)
{
    if (strcmp(scheduler, "SP") == 0)
    {
        stype = SP;
        ready_queue_sp = (sp_queue *)malloc(sizeof(sp_queue));
        ready_queue_sp->size = 0;
    }
    else
    {
        stype = DP;
        ready_queue_sp = (sp_queue *)malloc(sizeof(sp_queue));
        ready_queue_sp->size = 0;
        va_list valist;  // get variable args if DP
        va_start(valist, quantum);
        dp_x = va_arg(valist, int);
        va_end(valist);
    }

    f_queue = (fin_queue *)malloc(sizeof(fin_queue));
    f_queue->front = 0;
    f_queue->size = 0;
    f_queue->rear = 0;	

    memset(&timer_action, 0, sizeof(timer_action));
    timer_action.sa_handler = &timer_handler;
    timer_action.sa_flags = 0;

    if(sigemptyset(&(timer_action.sa_mask)) == -1)
    {
        logger("Signal Empty",LOG_ERROR);
        return -1;
    }

    if(sigaddset(&(timer_action.sa_mask), SIGVTALRM) == -1)
    {
        logger("Signal add",LOG_ERROR);
        return -1;
    }

    if (sigaction(SIGVTALRM, &timer_action, NULL) == -1)
    {
        logger("sigaction",LOG_ERROR);
        return -1;
    }

    if(set_timer(quantum) == -1)
    {
        logger("Set timer",LOG_ERROR);
        return -1;
    }

    struct ucontext_t main_context;
    if (getcontext(&main_context) == -1) {
        logger("main thread context",LOG_ERROR);
        return -1;
    }

    TCB *main_TCB = malloc(sizeof(TCB));
    main_TCB->id_ = MAIN_THREAD;
    main_TCB->state_ = RUNNING;
    main_TCB->context_ = main_context;
    main_TCB->priority_ = 10; // assumed main thread should have low priority
    t_running = main_TCB;
    threads[0] = main_TCB;

    return 0;
}

int thread_create(void(*start_routine), void *arg, ...)
{
    int id = get_thread_id(threads);
    if (id == -1)
    {
        logger("Max Threads",LOG_ERROR);
        return -1;
    }

    int prio;
    va_list valist;  // get variable args if SP
    va_start(valist, arg);
    prio = va_arg(valist, int);

    if (prio < 1)
        prio = 1;
    else if(prio > 10)
        prio = 10;

    va_end(valist);

    TCB *thread_TCB = (TCB *) malloc(sizeof(TCB)); 
    thread_TCB->id_ = id;
    threads[id] = thread_TCB; 

    struct ucontext_t thread_context;
    thread_context.uc_stack.ss_sp = (char *)malloc(sizeof(char) * STACK_SZ);
    thread_context.uc_stack.ss_size = STACK_SZ;
    thread_context.uc_stack.ss_flags = 0; 

    thread_TCB->priority_ = prio;
    enqueue_sp(ready_queue_sp, thread_TCB);

    if (getcontext(&thread_context) == -1) {
        logger("Get Thread Context",LOG_ERROR);
        goto free_tcb;
    }

    makecontext(&thread_context, (void (*)(void))stub, 2, start_routine, arg);
    thread_TCB->stack_ = thread_context.uc_stack.ss_sp;

    return id;

free_tcb: // free any memory incase of errors
    dequeue_id(ready_queue_sp, id);

    free(thread_TCB->stack_); 
    free(thread_TCB);  
    return -1;
}

int thread_yield(void)
{	
    // switch threads
    // don't explicitly dequeue since switch threads handles that
    if (stype == SP)
    {
        // only switch threads if main thread
        // this ensures a thread will run until quantum expires unless its main
        if (thread_self() == 0 && ready_queue_sp->size > 0)
        {
            if (switchThreads() == -1)
            {
                logger("main yeild switch threads",LOG_ERROR);
                return -1;
            }
        }
    }
    else
    {
        // Assume when a thread calls yeild during DP it gives up quantum
        // thus quant_count + 1
        quant_count++;
        if (ready_queue_sp->size > 0)
        {
            if (switchThreads() == -1)
            {
                logger("main yeild switch threads",LOG_ERROR);
                return -1;
            }
        }
    }
    return 0;
}


void thread_exit(void)
{

    int tid;
    if ((tid = thread_self()) == -1)
        logger("exit self",LOG_ERROR);

    // main thread exit
    if (tid == 0)
    {
        struct block_queue *curr = b_queue;
        struct block_queue *nex;

        while (curr != NULL)
        {
            nex = curr->next;
            free(curr);
            curr = nex;
        }
        free(f_queue);
        free(ready_queue_sp);

    }
    else
    {
        if (disable_interrupt(&timer_action.sa_mask) == -1)
            logger("exit disable inturrpt",LOG_ERROR);

        struct join_queue *curr = j_queue;
        while(curr != NULL)
        {
            if (curr->wait_on == tid)
            {
                enqueue_fin(f_queue, threads[tid]);
                break;
            }
            curr = curr->next;
        }
        threads[tid]->state_ = FINISHED;

        if (enable_interrupt(&timer_action.sa_mask) == -1)
            logger("exit enable inturrpt",LOG_ERROR);	

        // switch threads	

        if (ready_queue_sp->size > 0)
        {
            if (switchThreads() == -1)
                logger("exit switch threads",LOG_ERROR);
        }
    }
}

int thread_suspend(int tid)
{

    enqueue_block(b_queue, threads[tid]);

    if (threads[tid]->state_ == READY)
    {
        if (disable_interrupt(&timer_action.sa_mask) == -1)
            logger("suspend disable inturrpt",LOG_ERROR);

        dequeue_id(ready_queue_sp, tid);
        threads[tid]->state_ = BLOCKED;

        if (enable_interrupt(&timer_action.sa_mask) == -1)
            logger("exit enable inturrpt",LOG_ERROR);

    } 
    else if (threads[tid]->state_ == RUNNING)
    {
        threads[tid]->state_ = BLOCKED;
        quant_count++;	
        if (switchThreads() == -1)
        {
            logger("suspend switch threads",LOG_ERROR);
            return -1;
        }
    }
    return 0;
}

int thread_resume(int tid)
{	
    if (disable_interrupt(&timer_action.sa_mask) == -1)
    {
        logger("resume disable inturrpt",LOG_ERROR);
        return -1;
    }

    dequeue_block(b_queue, tid);
    threads[tid]->state_ = READY;

    enqueue_sp(ready_queue_sp, threads[tid]);

    if (enable_interrupt(&timer_action.sa_mask) == -1)
    {
        logger("suspend enable inturrpt",LOG_ERROR);
        return -1;
    }
    return 0;

}

int thread_join(int tid)
{
    if (disable_interrupt(&timer_action.sa_mask) == -1)
    {
        logger("join disable inturrpt",LOG_ERROR);
        return -1;
    }

    int stid = thread_self();

    if (stid == -1){
        return -1;
    }


    if (threads[tid]->state_ != FINISHED)
    {

        enqueue_join(j_queue, threads[stid], tid);	
        threads[stid]->state_ = BLOCKED;

        if (enable_interrupt(&timer_action.sa_mask) == -1)
        {
            logger("join enable inturrpt",LOG_ERROR);
            return -1;
        }

        // busy waiting
        // didn't know any other way to constantly check thread state		
        switchThreads();
        if (threads[tid]->state_ == FINISHED)
        {
            free(threads[tid]);
            threads[tid] = NULL;
            thread_num--;
        }

    }
    else
    {
        free(threads[tid]);
        threads[tid] = NULL;
        thread_num--;

        if (enable_interrupt(&timer_action.sa_mask) == -1)
        {
            logger("join enable inturrpt",LOG_ERROR);
            return -1;
        }
    }

    // remove from join queue
    struct join_queue *curr = j_queue;
    struct join_queue *next;

    int count;
    while (curr != NULL)
    {
        count++;
        next = curr->next;
        if (curr->wait_on == tid)
        {
            free(curr);
            curr = next;
            break;
        }
        curr = next;
    }

    // check if last thread in join queue
    // Main is the only thread joining, since it doesn't start in a stub,
    // explicitly call exit.
    if (count == 1)
        thread_exit();

    return 0;
}

int thread_self(void)
{
    return t_running != NULL ? t_running->id_ : -1;
}


int thread_priority(int tid)
{
    return threads[tid] != NULL ? threads[tid]->priority_ : -1;
}


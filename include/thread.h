/*
    Header: thread.h
    Author: Atakilt Massengill
*/

#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include "logger.h"

#define MAIN_THREAD 0   // Main thread ID

/******************************/
/*     Context Switch         */
/******************************/
/*
* Function:
*   Switches current running thread to next thread in ready queue. Also adds the current thread to ready queue, if it's not FINISHED.
* Arguments:
*   None
* Returns:
*   1 for success, -1 for failure
*/
int switchThreads(void);

/******************************/
/*           Timer            */
/******************************/

/* Function:
 *  Unblock timmer inturrpt
 * Args:
 *  Signal set for timer
 * Returns:
 *  0 for success, -1 for failure
*/
int enable_interrupt(const sigset_t *set);

/* Function:
 *  Block timmer inturrpt
 * Args:
 *  Signal set for timer
 * Returns:
 *  0 for success, -1 for failure
*/
int disable_interrupt(const sigset_t *set);

/* Function:
 *  Set up intial timer interrupt and handler
 * Args:
 *  Quantum defining amount of time inbetween signals
 * Returns:
 *  0 for success, -1 for failure
*/
int set_timer(int quantum);
/* Function:
 *  Set up intial timer interrupt and handler
 * Args:
 *  Quantum defining amount of time inbetween signals
 * Returns:
 *  0 for success, -1 for failure
*/
int reset_timer(void);

void timer_handler(int sig); // Timer handler for timer inturrpt

/******************************/
/*           API              */
/******************************/
/*
* Function:
*   thread_init
* Arguments:
*   scheduler   : RR/SP/DP
*   quantum     : User input for quantum in microseconds
* Returns:
*   0 on success, -1 on failure
*/
int thread_init(char *scheduler, int quantum, ...);

/*
* Function:
*   thread_create
* Arguments:
*   start_routine   : The routine a thread has to run
*   arg             : Arguments required by the routine
* Returns:
*   Thread id on success, -1 on failure
*/
int thread_create(void(*start_routine), void *arg, ...);

/*
* Function:
*   thread_join
* Arguments:
*   tid   : ID of thread that the calling thread want to join
* Returns:
*   0 on success, -1 on failure
*/
int thread_join(int tid);

/*
* Function:
*   thread_yield
* Arguments:
*   NA
* Returns:
*   0 on success, -1 on failure
*/
int thread_yield(void);

/*
* Function:
*   thread_exit
* Arguments:
*   NA
* Returns:
*   0 on success, -1 on failure
*/
void thread_exit(void);

/*
* Function:
*   thread_suspend
* Arguments:
*  tid  : Calling thread suspends thread with id tid
* Returns:
*   0 on success, -1 on failure
*/
int thread_suspend(int tid);

/*
* Function:
*   thread_resume
* Arguments:
*  tid  : Calling thread resumes thread with id tid
* Returns:
*   0 on success, -1 on failure
*/
int thread_resume(int tid);

/*
* Function:
*   thread_self
* Arguments:
*  NA
* Returns:
*   tid of running thread on success, -1 on failure
*/
int thread_self(void);

/*
* Function:
*   thread_priority
* Arguments:
*  tid: ID of thread whose priority should be retrieved
* Returns:
*   priority of thread tid on success, -1 on failure
*/
int thread_priority(int tid);

#endif

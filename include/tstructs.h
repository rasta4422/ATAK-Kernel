/*
    Header: tstructs.h
    Author: Atakilt Massengill
*/

#ifndef TSTRUCTS_H
#define TSTRUCTS_H

#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ucontext.h>
#include "logger.h"

/*****************************/
#define MAX_THREADS 100
#define STACK_SZ 4096
#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

typedef enum {
    SP, // Static priority
    DP  // Dynamic priority
} schedulerType;

extern schedulerType stype;
/******************************/
/*           TCB              */
/******************************/
typedef enum {
    READY,
    RUNNING,
    BLOCKED,
	FINISHED
} threadState;

typedef struct {
    int id_;                // thread id
    threadState state_;     // thread state
    ucontext_t context_;    // thread context (stores registers, pc)
    char *stack_;           // thread stack
    int priority_;          // thread priority
} TCB;

extern TCB *t_running; // keeps track of running thread
extern TCB *threads[MAX_THREADS];   // array placeholder for TCBs; threads[0] is always the main thread

/*
* Functionality:
*   Run the start_routine with argument arg
*   Call uthread_exit
*   Enable and disable timers as required -> Think about this
*   You may move the stub function to other files
*/
void stub(void(*start_routine)(void *), void *arg);


/*****************************/
/*          SP               */
/*****************************/

typedef struct {
    int size;                   // curr size of heap
    TCB *heap[MAX_THREADS];     // priority queue heap
} sp_queue;

extern sp_queue *ready_queue_sp;    // ready queue for SP

/* Function:
 *  Swaps threads in the heap
 * Args:
 *  The two thread TCBs to be swaped
*/
void swap(int i1, int i2, sp_queue *q);


/* Function:
 *  Heapify the min-heap priority queue
 * Args:
 *  q: Queue to heapify
 *  i: index of root node
*/
void heapify(sp_queue *q, int i);

/* Function:
 *  enqueue the element to the heap
 * Args:
 *  q: Queue to add element to
 *  t: TCB of element to add
*/
void enqueue_sp(sp_queue *q, TCB *t);

/* Function:
 *  dequeue highest priority thread from heap
 * Args:
 *  q: Queue to extract TCB from
 * Returns:
 *  TCB of root node (i.e. highest priority thread)
*/
TCB *dequeue_sp(sp_queue *q);

/* Function:
 *  dequeue specific thread from heap in case of errors
 * Args:
 *  q: Queue to extract TCB from
 *  id: Thread ID
 * Returns:
 *  TCB of the thread ID
*/
TCB *dequeue_id(sp_queue *q, int id);


/*****************************/
/*          DP               */
/*****************************/

/* Function:
 *  Change the priority of elements in the sp queue, and the running thread
 * Args:
 *  q: SP queue
 *  t: current running thread
*/
void change_priority(sp_queue *q, TCB *t);


/*****************************/
/*          JOIN             */
/*****************************/
typedef struct join_queue {
	int wait_on;					// ID of thread "th" is waiting on
    struct join_queue *next;		// next element in list
	TCB *th;						// TCB of the thread who called join
} join_queue;


/* Function:
 *  Enqueue the element to join queue
 * Args:
 *  q: Queue to add element to
 *  t: TCB of element to add
*/
void enqueue_join(struct join_queue *curr, TCB *t, int tid_w);

/*****************************/
/*          FINISHED         */
/*****************************/
typedef struct {
	int rear;					// index to add to
    int front;					// index to remove from
	int size;                  	// current size 
    TCB *array[MAX_THREADS];    // queue array
} fin_queue;

/* Function:
 *  Enqueue the element to finished queue
 * Args:
 *  q: Queue to add element to
 *  t: TCB of element to add
*/
void enqueue_fin(fin_queue *q, TCB *t);

/*****************************/
/*          BLOCK            */
/*****************************/
typedef struct block_queue {
    struct block_queue *next;	// pointer to next element
	TCB *th;					// TCB info of each element
} block_queue;

/* Function:
 *  Enqueue the element to block queue
 * Args:
 *  q: Queue to add element to
 *  t: TCB of element to add
*/
void enqueue_block(struct block_queue *curr, TCB *t);

/* Function:
 *  Dequeue the element from block queue
 * Args:
 *  q: Queue to dequeue element from
 *  id: ID of the TCB
*/
void dequeue_block(struct block_queue *curr, int id);

#endif

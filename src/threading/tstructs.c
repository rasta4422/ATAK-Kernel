#include "tstructs.h"

void swap(int i1, int i2, sp_queue *q)
{
    TCB *tmp = q->heap[i2];
    q->heap[i2] = q->heap[i1];
    q->heap[i1] = tmp;
}

void heapify(sp_queue *q, int i)
{
    if (q->size != 1)
    {
        int b = i;
        int l = (i*2) + 1;
        int r = (i*2) + 2;
        if (l < q->size && q->heap[l]->priority_ < q->heap[b]->priority_)
            b = l;
        if (r < q->size && q->heap[r]->priority_ < q->heap[b]->priority_)
            b = r;
        if (b != i)
        {
            swap(b, i, q);
            heapify(q, b);
        }
    }
}


void enqueue_sp(sp_queue *q, TCB *t)
{
    if (q->size == 0)
    {
        q->heap[q->size++] = t;
    } 
    else
    {
        q->heap[q->size++] = t;
        heapify(q, 0);
    }
}


TCB *dequeue_sp(sp_queue *q) 
{
    if (q->size == 0)
    {
        return NULL;
    } 
    else if (q->size == 1)
    {
        return q->heap[--q->size];	
    } 
    else 
    {
        swap(0, --q->size, q);
        heapify(q,0);
        return q->heap[q->size];
    }
}


TCB *dequeue_id(sp_queue *q, int id)
{
    if (q->size == 0)
    {
        return NULL;
    } 
    else if (q->size == 1)
    {
        return q->heap[--q->size];
    } 
    else
    {
        int i;
        // find id index in heap
        for (i=0; i<q->size;i++)
        {
            if (q->heap[i]->id_ == id)
                break;
        }
        swap(i, --q->size, q);
        heapify(q,0);
        return q->heap[q->size];
    }
}


void change_priority(sp_queue *q, TCB *t)
{
    for(int i=0;i<q->size;i++)
    {
        if (q->heap[i]->priority_ > 1)
            q->heap[i]->priority_--;
    }

    if (t->priority_ < 10)
        t->priority_++;

    heapify(q, 0);
}



void enqueue_join(struct join_queue *j, TCB *t, int tid_w)
{
    struct join_queue *curr = j;
    if (curr == NULL)
    {
        curr = (struct join_queue *)malloc(sizeof(join_queue));
        curr->th = t;
        curr->wait_on = tid_w;
        curr->next = NULL;
        j = curr;
    }
    else
    {
        while(curr->next != NULL)
        {
            curr = curr->next;
        }
        curr->next = (struct join_queue *)malloc(sizeof(join_queue));
        curr = curr->next;
        curr->th = t;
        curr->wait_on = tid_w;
        curr->next = NULL;
    }
}

void enqueue_fin(fin_queue *q, TCB *t)
{
    q->array[q->rear] = t;
    q->rear = (q->rear + 1) % MAX_THREADS;
    q->size++;
}


void enqueue_block(struct block_queue *b, TCB *t)
{
    struct block_queue *curr = b;
    if (curr == NULL)
    {
        curr = (struct block_queue *)malloc(sizeof(block_queue));
        curr->th = t;
        curr->next = NULL;
        b = curr;
    }
    else
    {
        while(curr->next != NULL)
        {
            curr = curr->next;
        }
        curr->next = (struct block_queue *)malloc(sizeof(block_queue));
        curr = curr->next;
        curr->th = t;
        curr->next = NULL;
    }
}

void dequeue_block(struct block_queue *b, int id)
{
    struct block_queue *curr = b;
    struct block_queue *prev = NULL;

    while (curr != NULL)
    {
        if (curr->th->id_ == id)
        {
            if (prev == NULL)
                b = curr->next;
            else 
                prev->next = curr->next;

            return;
        }
        prev = curr;
        curr = curr->next;
    }	
}

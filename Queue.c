/*
 * Queue.c
 *
 *  Created on: Sep 24, 2019
 *      Author: Ray
 */

#include "Queue.h"
#include "Uart.h"

struct Queue InQ;
struct Queue OutQ;

void Queue_Init()
{
    InQ.Head=0;
    InQ.Tail=0;
    OutQ.Head=0;
    OutQ.Tail=0;
}

int EnQueue(enum QueueType t, enum Source s, char v)
{
    int head;
    int rtv = FALSE;
    //InterruptMasterDisable();
    switch (t)
    {
        case INPUT:
        {
            head = InQ.Head;
            if((head+1)&QSM1 != InQ.Tail) // if not full
            {
                (InQ.queue[head]).value = v;
                InQ.queue[head].source = s;
                InQ.Head=(head+1)&QSM1;
                rtv = TRUE;
            }
            break;
        }
        case OUTPUT:
        {
            head = OutQ.Head;
            if((head+1)&QSM1 != OutQ.Tail)  // if not full
            {
                OutQ.queue[OutQ.Head].value = v;
                OutQ.Head=(head+1)&QSM1;
                rtv = TRUE;
            }
            break;
        }
    }
    //InterruptMasterEnable();
    return rtv;
}

int DeQueue(enum QueueType t, enum Source* s, char* v)
{
    int tail;
    switch (t)
    {
        case INPUT:
        {
            tail = InQ.Tail;
            if(InQ.Head!=tail)  // if not empty
            {
                *s=InQ.queue[tail].source;
                *v=InQ.queue[tail].value;
                InQ.Tail = (tail+1)&QSM1;    // Update tail
            }
            else    // if empty
                return FALSE;

            break;
        }

        case OUTPUT:
        {
            tail = OutQ.Tail;
            if(OutQ.Head!=tail)  // if not empty
            {
                *v=OutQ.queue[tail].value;
                OutQ.Tail = (tail+1)&QSM1;    // Update tail
            }
            else    // if empty
                return FALSE;

            break;
        }
    }
    return TRUE;
}

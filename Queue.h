/*
 *  Queue.h
 *
 *  Created on: Sep 24, 2019
 *      Author: Ray Su
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#define TRUE    1
#define FALSE   0
#define QUEUESIZE 8 // the size of queue

/* The source of input data
 * Output data is all UART type*/
enum Source
{
    UART,
    SYSTICK
};

/* Structure of data in queue
 * include Source of data and value of data
 * Data from Systick only have data type "SYSTICK"
 * Output data only have UART type*/
struct QueueData
{
    enum Source source;
    char value;
};

/* Structure of queue*/
struct Queue
{
    int Head; // the head of queue
    int Tail; // the tail of queue
    struct QueueData* queue[QUEUESIZE];  // the queue stores data
};

int EnQueue(enum Source s, char v);
int DeQueue(enum Source s, char* v);

#endif /* QUEUE_H_ */

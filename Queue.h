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
#define QSM1 QUEUESIZE-1 // Queue size minus one

/* The source of input data
 * Output data is all UART type*/
typedef enum
{
    UART,
    SYSTICK
} Source;

/* The name of queue
 * Only two queues exist
 * Input queue and output queue*/
typedef enum
{
    INPUT,
    OUTPUT
} QueueType;

/* Structure of data in queue
 * include Source of data and value of data
 * Data from Systick only have data type "SYSTICK"
 * Output data only have UART type*/
typedef struct
{
    Source source;
    char value;
} QueueData;

/* Structure of queue*/
typedef struct
{
    int Head; // the head of queue
    int Tail; // the tail of queue
    QueueData queue[QUEUESIZE];  // the queue stores data
} Queue;

void Queue_Init();  // Initialize input queue and output queue by setting the head and tail to 0
int EnQueue(QueueType t, Source s, char v);   //Enqueue a data to a queue
int DeQueue(QueueType t, Source* s, char* v);  //Dequeue a data from a queue

#endif /* QUEUE_H_ */

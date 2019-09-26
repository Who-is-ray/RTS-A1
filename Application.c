/*
 * Application.c
 *
 *  Created on: Sep 26, 2019
 *      Author: Ray
 */

#include "Application.h"
#include "Queue.h"
#include "Uart.h"

/* Initialization Uart, Systick and Queues
 * Enable interrupts*/
void Initialization()
{
    /* Initialize UART */
    UART0_Init();           // Initialize UART0
    Queue_Init();           // Initialize Queues
    InterruptEnable(INT_VEC_UART0);       // Enable UART0 interrupts
    UART0_IntEnable(UART_INT_RX | UART_INT_TX); // Enable Receive and Transmit interrupts
    InterruptMasterEnable();      // Enable Master (CPU) Interrupts
}

void Run()
{
    Initialization();

    // for test, ">" sign
       UART0_DR_R = 62;

       while(1)
       {
           /* Input data - xmit directly */
           struct QueueData data;
           if(DeQueue(INPUT,&data.source,&data.value) == TRUE) // If input is not empty
           {
               while(EnQueue(OUTPUT, data.source, data.value)==FALSE){} // wait if output queue is full
           }
       }
}

/**
 * Real Time System Assignment 1
 *
 * Author: Ray Su B00634512
 * Fall 2019
 */

#include <stdio.h>

#include "Queue.h"
#include "Uart.h"

void main (void)
{
    /* Initialize UART */
    UART0_Init();           // Initialize UART0
    Queue_Init();           // Initialize Queues
    InterruptEnable(INT_VEC_UART0);       // Enable UART0 interrupts
    UART0_IntEnable(UART_INT_RX | UART_INT_TX); // Enable Receive and Transmit interrupts
    InterruptMasterEnable();      // Enable Master (CPU) Interrupts
    UART0_DR_R = 60;
    while(1)
    {
        /* Input data - xmit directly */
        struct QueueData data;
        if(DeQueue(INPUT,&data.source,&data.value) == TRUE)
        {
            while(EnQueue(OUTPUT, data.source, data.value)==FALSE){}
        }
    }

}

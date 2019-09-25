/**
 * Real Time System Assignment 1
 *
 * Author: Ray Su B00634512
 * Fall 2019
 */

#include <stdio.h>

#include "Queue.h"
#include "Uart.h"

/* Globals */
volatile char Data = 7;     /* Input data from UART receive */
volatile int GotData;   /* T|F - Data available from UART */
volatile struct Queue InQ; /* Queue of input data*/

void main (void)
{
    /* Initialize UART */
    UART0_Init();           // Initialize UART0
    InterruptEnable(INT_VEC_UART0);       // Enable UART0 interrupts
    UART0_IntEnable(UART_INT_RX | UART_INT_TX); // Enable Receive and Transmit interrupts
    InterruptMasterEnable();      // Enable Master (CPU) Interrupts
    UART0_DR_R = 60;
    while(1)
    {
        /* Wait for input data */
        GotData = FALSE;
        while (!GotData) ;
        /* Input data - xmit directly */
        UART0_DR_R = Data;
    }

}

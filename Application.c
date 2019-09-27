/*
 * Application.c
 *
 *  Created on: Sep 26, 2019
 *      Author: Ray
 */

#include <string.h>
#include "Application.h"
#include "Queue.h"
#include "Uart.h"

#define STRING_SIZE 64  // size of string to store
#define ALARM_TIME_COMD_SIZE    16
#define COMMON_CHAR_START   32    // Char can direct echo start from 32(' ')
#define COMMON_CHAR_END 126     // Char can direct echo end at 126('~')
#define ALPHABET_LC_START   97   // alphabet lower case character start from 97('a')
#define ALPHABET_LC_END 122   // alphabet lower case character end at 122('z')
#define ALPHABET_CASE_OFFSET  0b11011111    // and value upper case
#define VERTICAL_TAB   11
#define ENTER   13
#define BACKSPACE   127
#define EQUAL 0

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

void OutputNewLine()
{
    while(EnQueue(OUTPUT, UART, ENTER)==FALSE){}
    while(EnQueue(OUTPUT, UART, VERTICAL_TAB)==FALSE){}
    //while(EnQueue(OUTPUT, UART, 27)==FALSE){}
    //while(EnQueue(OUTPUT, UART, 91)==FALSE){}
    //while(EnQueue(OUTPUT, UART, 67)==FALSE){}
}

void Run()
{
    Initialization();

    // for testing, ">" sign
    UART0_DR_R = 62;

    char str[STRING_SIZE];
    int str_counter = 0;
    int is_echo = FALSE;    // flag to indicate if ehco back

    while(1)
    {
       /* Input data - xmit directly */
       struct QueueData data;
       if(DeQueue(INPUT,&data.source,&data.value) == TRUE) // If input is not empty
       {
           char data_val=data.value;

           /*process the input*/
           if(data_val>=COMMON_CHAR_START && data_val<=COMMON_CHAR_END) // if data is common char
           {
               if(str_counter<STRING_SIZE-1) // if string not full
               {
                   // store to string and echo back
                   if(data_val<=ALPHABET_LC_END) // if could be a lower case letter
                       data_val &= ALPHABET_CASE_OFFSET;    // convert to upper case letter

                   str[str_counter]=data_val;
                   str_counter++;
                   is_echo = TRUE;
               }
           }
           else if(data_val == BACKSPACE)   // if data is backspace
           {
               if(str_counter >0)    // if string not empty
               {
                   str_counter--;   // remove last bit from string
                   is_echo = TRUE;
               }
           }
           else if(data_val == ENTER)   // if data is enter
           {
               OutputNewLine(); // Change to new line

               /* Process the string */
               if (strncmp(str, "TIME", 4)==EQUAL) // If start with 'TIME'
               {
                   //
               }

               str_counter = 0; // Clear the string
           }

           /*echo if need to*/
           if(is_echo == TRUE)
           {
               while(EnQueue(OUTPUT, data.source, data.value)==FALSE){} // wait if output queue is full
               is_echo = FALSE;
           }
       }
    }
}

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

#define STRING_SIZE             64          // size of string to store
#define ALARM_PARA_CMD_SIZE     16          // size of alarm cmd with parameter
#define ALARM_CMD_SIZE          5           // size of alarm cmd
#define TIME_CMD_SIZE           4           // size of time cmd
#define TIME_PARA_CMD_SIZE      15          // size of time cmd with parameter
#define DATE_CMD_SIZE           4           // size of date cmd
#define DATE_PARA_CMD_SIZE1     15          // size of date cmd with parameter of 1 digit date
#define DATE_PARA_CMD_SIZE2     16          // size of date cmd with parameter of 2 digits date
#define MAX_SEC                 59          // max value of second
#define MAX_MIN                 59          // max value of minute
#define MAX_HOUR                23          // max value of hour
#define NUM_OF_MON              12          // number of month
#define NUM_OF_CHAR_IN_MON      3           // number of letter in month
#define NUM_TYPE_OF_MON         2           // leap year month and normal year month
#define LEAP_YEAR_PERIOD        4           // leap year period

// ASCII Table Define
#define COMMON_CHAR_START       32          // char can direct echo start from 32(' ')
#define COMMON_CHAR_END         126         // char can direct echo end at 126('~')
#define NUMBER_START            48          // number start from 47(0)
#define NUMBER_END              57          // number start from 58(0)
#define ALPHABET_UC_START       65          // alphabet upper case character start from 122('A')
#define ALPHABET_UC_END         90          // alphabet lower case character end at 122('Z')
#define ALPHABET_LC_START       97          // alphabet lower case character start from 97('a')
#define ALPHABET_LC_END         122         // alphabet lower case character end at 122('z')
#define ALPHABET_CASE_OFFSET    0b11011111  // and value upper case
#define VERTICAL_TAB            11
#define ENTER                   13
#define SPACE                   32
#define DASH                    45
#define PERIOD                  46
#define COLON                   58
#define GREATER_THAN            62
#define QUESTION_MARK           63
#define BACKSPACE               127
#define ESC                     27
#define EQUAL                   0

char str[STRING_SIZE];  // command string
int str_counter = 0;    // command string letter counter
int is_ESC_seq = FALSE; // flag to indicate if is ESC sequences
int t_sec,  //1/10 of second
    sec,    // second
    min,    // minute
    hour,   // hour
    day,    // day
    mon_int,// month integer
    year = 0; // year
char mon_str[NUM_OF_CHAR_IN_MON]; //month string
const char mon_list[NUM_OF_MON][NUM_OF_CHAR_IN_MON]={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"}; // list of month
const int days_list[NUM_TYPE_OF_MON][NUM_OF_MON] = {  // list of possible day list
                              {31,28,31,30,31,30,31,31,30,31,30,31}, // list of days in month,days_in_month_ly
                              {31,29,31,30,31,30,31,31,30,31,30,31}}; // list of days in month in leap year

//****************************************************************************
//
// External declarations for the interrupt handlers used by the application.
//
//*****************************************************************************

/* Transmit a character*/
void TransChar(char c)
{
    while(EnQueue(OUTPUT, UART, c)==FALSE);
}

/* Output a string*/
void OutputString(const char* s)
{
    int i;
    for(i = 0; i <= strlen(s); i++) // output each character in string
        {while(EnQueue(OUTPUT, UART, s[i])==FALSE);}
}

/* Move cursor to new line*/
void OutputNewLine()
{
    TransChar(ENTER);
    TransChar(VERTICAL_TAB);
}

/* Move cursor to new line and output prefix*/
void OutputNewLinePrefix()
{
    TransChar(ENTER);
    TransChar(VERTICAL_TAB);
    OutputString("> ");
}

/* Indicate if the character is a number*/
int IsNumber(char c)
{
    if(c>=NUMBER_START && c<=NUMBER_END)
        return TRUE;
    return FALSE;
}

/* Indicate if the character is an upper case letter*/
int IsUCLetter(char c)
{
    if(c>=ALPHABET_UC_START && c<=ALPHABET_UC_END)
        return TRUE;
    return FALSE;
}

/*Check if date valid*/
int IsDateVaild(int y/*year*/, int m/*month*/, int d/*day*/)
{
    return ((days_list[(y%LEAP_YEAR_PERIOD)>0? FALSE:TRUE][m-1])<d)? FALSE:TRUE;
}

/* Decoding the time from command
 * return true if invalid*/
int DecodeTime(char str[], int count)
{
    if(IsNumber(str[count-1])) // test last bit: 1/10 of second
    {
        t_sec = str[count-1] - NUMBER_START;
        count--;
    }
    else // if invalid
        return TRUE;

    if(str[count-1] == PERIOD) // test last second bit: period
        count--;
    else // if invalid
        return TRUE;

    if(IsNumber(str[count-1])) // test last thrid bit: second ones digit
    {
        sec = str[count-1] - NUMBER_START;
        count--;
    }
    else // if invalid
        return TRUE;

    if(IsNumber(str[count-1])) // test last forth bit: second tens digit
    {
        sec += (str[count-1] - NUMBER_START)*10;
        if(sec>MAX_SEC)  // if second is invalid
            return TRUE;
        count--;
    }
    else // if invalid
        return TRUE;

    if(str[count-1] == COLON) // test last fifth bit: colon
        count--;
    else // if invalid
        return TRUE;

    if(IsNumber(str[count-1])) // test last sixth bit: minute ones digit
    {
        min = str[count-1] - NUMBER_START;
        count--;
    }
    else // if invalid
        return TRUE;

    if(IsNumber(str[count-1])) // test last seventh bit: minute tens digit
    {
        min += (str[count-1] - NUMBER_START)*10;
        if(min>MAX_MIN)  // if minute is invalid
            return TRUE;
        count--;
    }
    else // if invalid
        return TRUE;

    if(str[count-1] == COLON) // test last eighth bit: colon
        count--;
    else // if invalid
        return TRUE;

    if(IsNumber(str[count-1])) // test last ninth bit: hour ones digit
    {
        hour = str[count-1] - NUMBER_START;
        count--;
    }
    else // if invalid
        return TRUE;

    if(IsNumber(str[count-1])) // test last tenth bit: hour tens digit
    {
        hour += (str[count-1] - NUMBER_START)*10;
        if(hour>MAX_HOUR)  // if minute is invalid
            return TRUE;
        count--;
    }
    else // if invalid
        return TRUE;

    if(str[count-1] == SPACE) // test last eleventh bit: space
        return FALSE;

    return TRUE;
}

/* Decoding the time from command
 * return true if invalid*/
int DecodeDate(char str[], int count)
{
    if(IsNumber(str[count-1])) // test last bit: ones digit of year
    {
        year = str[count-1] - NUMBER_START;
        count--;
    }
    else // if invalid
        return TRUE;

    if(IsNumber(str[count-1])) // test last second bit: tens digit of year
    {
        year += (str[count-1] - NUMBER_START)*10;
        count--;
    }
    else // if invalid
        return TRUE;

    if(IsNumber(str[count-1])) // test last thrid bit: hundreds digit of year
    {
        year += (str[count-1] - NUMBER_START)*100;
        count--;
    }
    else // if invalid
        return TRUE;

    if(IsNumber(str[count-1])) // test last forth bit: thousands digit of year
    {
        year += (str[count-1] - NUMBER_START)*1000;
        count--;
    }
    else // if invalid
        return TRUE;

    if(str[count-1] == DASH) // test last fifth bit: dash
        count--;
    else // if invalid
        return TRUE;

    int i;
    for(i=NUM_OF_CHAR_IN_MON-1; i>=0; i--) // read month
    {
        mon_str[i] = str[count-1];
        count--;
    }
    int not_match = TRUE; // flag to indicate if month exist
    for(mon_int=1; mon_int<=NUM_OF_MON; mon_int++) // find month in month list
    {
        if(strncmp(mon_str, mon_list[mon_int-1],NUM_OF_CHAR_IN_MON) == EQUAL) // if find in month list
        {
            not_match = FALSE;
            break;
        }
    }
    if(not_match) // if not a valid month
        return TRUE;

    if(str[count-1] == DASH) // test last ninth bit: dash
        count--;
    else // if invalid
        return TRUE;

    if(IsNumber(str[count-1])) // test last tenth bit: last digit of date
    {
        day = str[count-1] - NUMBER_START;
        count--;
    }
    else // if invalid
        return TRUE;

    if(IsNumber(str[count-1])) // test last eleventh bit: first digit of date
    {
        day += (str[count-1] - NUMBER_START)*10;

        if(IsDateVaild(year,mon_int,day)==FALSE) // check if date is invalid
            return TRUE;

        count--;
        if(str[count-1] == SPACE) // test last twelfth bit: space
            return FALSE;
    }
    else if(str[count-1] == SPACE || strlen(str)==DATE_PARA_CMD_SIZE1) // test last eleventh bit: space
        return FALSE;
    return TRUE;
}

/* Initialization Uart, Systick and Queues
 * Enable interrupts*/
void Initialization()
{
    /* Initialize UART */
    UART0_Init();           // Initialize UART0
    Queue_Init();           // Initialize Queues
    InterruptEnable(INT_VEC_UART0);       // Enable UART0 interrupts
    UART0_IntEnable(UART_INT_RX | UART_INT_TX); // Enable Receive and Transmit interrupts
    InterruptMasterEnable();    // Enable Master (CPU) Interrupts
    OutputString("> ");     // Output first pre-fix
}
/* check if input queue has data to process
 * process if has*/
void CheckInputQueue()
{
    int need_echo = FALSE;    // flag to indicate if ehco back

    /* Input data - xmit directly */
    QueueData data;
    if(DeQueue(INPUT,&data.source,&data.value) == TRUE) // If input is not empty
    {
       char data_val=data.value;
       int has_error = FALSE; // flag indicate if cmd has error

       /*process the input*/
       if(is_ESC_seq == TRUE)   // if is a part of ESC sequence
       {
           char data_val_uc = data_val & ALPHABET_CASE_OFFSET;    // convert to upper case letter
           if(data_val_uc>=ALPHABET_UC_START && data_val_uc<=ALPHABET_UC_END)
               is_ESC_seq = FALSE;  // most case ESC sequences ended with a letter
           need_echo = TRUE;
       }
       else if(data_val>=COMMON_CHAR_START && data_val<=COMMON_CHAR_END) // if data is common char
       {
           if(str_counter<STRING_SIZE) // if string not full
           {
               // store to string and echo back
               if((data_val>=ALPHABET_LC_START) && (data_val<=ALPHABET_LC_END)) // if is a lower case letter
                   data_val &= ALPHABET_CASE_OFFSET;    // convert to upper case letter

               str[str_counter]=data_val;
               str_counter++;
               need_echo = TRUE;
           }
       }
       else if(data_val == BACKSPACE)   // if data is backspace
       {
           if(str_counter >0)    // if string not empty
           {
               str_counter--;   // remove last bit from string
               need_echo = TRUE;
           }
       }
       else if(data_val == ENTER)   // if data is enter
       {
           OutputNewLine(); // change to new line
           int count = str_counter; // save string count to local
           str_counter = 0; // reset the string counter

           /* Process the string */
           if (strncmp(str, "TIME", TIME_CMD_SIZE)==EQUAL) // if start with 'TIME'
           {
               if(count == TIME_CMD_SIZE)// if has no parameter
               {
                   // if time has set, output time

                   // if time not set, output error
               }
               else if (count == TIME_PARA_CMD_SIZE) // if has parameters
               {
                   has_error = DecodeTime(str,count);
                   if(has_error == FALSE)
                   {
                       // set to systick

                       OutputNewLinePrefix();
                       return;
                   }
               }
               else // error
                   has_error = TRUE;
           }
           else if (strncmp(str, "DATE", DATE_CMD_SIZE)==EQUAL) // if start with 'DATE'
           {
               if(count == DATE_CMD_SIZE)// if has no parameter
               {
                   // if date has set, output time

                   // if date not set, output error
               }
               else if ((count == DATE_PARA_CMD_SIZE1) || (count == DATE_PARA_CMD_SIZE2)) // if has parameters
               {
                   has_error = DecodeDate(str,count);
                   if(has_error == FALSE)
                   {
                       // set to systick

                       OutputNewLinePrefix();
                       return;
                   }
               }
               else // error
                   has_error = TRUE;
           }
       }
       else if(data_val == ESC) // if data is ESC
       {
           is_ESC_seq = TRUE;
           need_echo = TRUE;
       }

       /*After processed command*/
       if(need_echo) // echo if need to
       {
           TransChar(data.value); // echo back
           need_echo = FALSE;
       }
       else if(has_error) // report error
       {
           TransChar(QUESTION_MARK);
           OutputNewLine();
           OutputString("> ");
       }
    }
}

//****************************************************************************
//
// Public functions
//
//*****************************************************************************

/* Run application*/
void Run()
{
    Initialization();

    while(1)
    {
        CheckInputQueue();
        // CheckAlarm();
    }
}

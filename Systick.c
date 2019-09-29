/*
 * Systick.c
 *
 *  Created on: Sep 28, 2019
 *      Author: Ray
 */

#include "Systick.h"
#include "Queue.h"
#include "Uart.h"

const int days_list[NUM_TYPE_OF_MON][NUM_OF_MON] = {  // list of possible day list
                              {31,28,31,30,31,30,31,31,30,31,30,31}, // list of days in month,days_in_month_ly
                              {31,29,31,30,31,30,31,31,30,31,30,31}}; // list of days in month in leap year


volatile Systick_Clock clock = {.alarm_cd    = DISABLED,
                                .day         = 1,
                                .month       = 9,
                                .year        = 2019,
                                .hour        = 0,
                                .min         = 0,
                                .sec         = 0,
                                .t_sec       =0};

void SysTickStart(void)
{
    // Set the clock source to internal and enable the counter to interrupt
    ST_CTRL_R |= ST_CTRL_CLK_SRC | ST_CTRL_ENABLE;
}

void SysTickStop(void)
{
    // Clear the enable bit to stop the counter
    ST_CTRL_R &= ~(ST_CTRL_ENABLE);
}

void SysTickPeriod(unsigned long Period)
{
    /*
     For an interrupt, must be between 2 and 16777216 (0x100.0000 or 2^24)
    */
    ST_RELOAD_R = Period - 1;  /* 1 to 0xff.ffff */
}

void SysTickIntEnable(void)
{
    // Set the interrupt bit in STCTRL
    ST_CTRL_R |= ST_CTRL_INTEN;
}

void SysTickIntDisable(void)
{
    // Clear the interrupt bit in STCTRL
    ST_CTRL_R &= ~(ST_CTRL_INTEN);
}

void SysTickHandler(void)
{
    EnQueue(INPUT, SYSTICK, NULL);
}

void SysTickInit()
{
    SysTickPeriod(MAX_WAIT);
    SysTickIntEnable();
    SysTickStart();
}

int IsDateVaild(int y/*year*/, int m/*month*/, int d/*day*/)
{
    return ((days_list[(y%LEAP_YEAR_PERIOD)>0? FALSE:TRUE][m-1])<d)? FALSE:TRUE;
}

void IncreaseDate()
{
    if((days_list[(clock.year%LEAP_YEAR_PERIOD)>0? FALSE:TRUE][clock.month-1]) == clock.day)
    {
        clock.day =0;
        if(clock.month == NUM_OF_MON)
        {
            clock.month = 0;
            clock.year++;
        }
    }
}

void Tick()
{
    // Update time and date
    if(clock.t_sec == MAX_T_SEC)
    {
        clock.t_sec = 0;
        if (clock.sec == MAX_SEC)
        {
            clock.sec = 0;
            if(clock.min == MAX_MIN)
            {
                clock.min = 0;
                if (clock.hour == MAX_HOUR)
                {
                    clock.hour = 0;
                    IncreaseDate();
                }
                else clock.hour++;
            }
            else clock.min++;
        }
        else clock.sec++;
    }
    else clock.t_sec++;

    // Update clock
    if(clock.alarm_cd>=0)
        clock.alarm_cd--;
}

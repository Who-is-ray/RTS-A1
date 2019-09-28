/*
 * Systick.c
 *
 *  Created on: Sep 28, 2019
 *      Author: Ray
 */

#include "Systick.h"
#include "Queue.h"

Systick_Clock Systick_clock = {.alarm_cd    = DISABLED,
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

/*
 * Systick.h
 *
 *  Created on: Sep 28, 2019
 *      Author: Ray
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

// SysTick Registers
// SysTick Control and Status Register (STCTRL)
#define ST_CTRL_R   (*((volatile unsigned long *)0xE000E010))
// Systick Reload Value Register (STRELOAD)
#define ST_RELOAD_R (*((volatile unsigned long *)0xE000E014))

// SysTick defines
#define ST_CTRL_COUNT       0x00010000  // Count Flag for STCTRL
#define ST_CTRL_CLK_SRC     0x00000004  // Clock Source for STCTRL
#define ST_CTRL_INTEN       0x00000002  // Interrupt Enable for STCTRL
#define ST_CTRL_ENABLE      0x00000001  // Enable for STCTRL

// Maximum period
#define MAX_WAIT            1677721     // 1/10 second

#define DISABLED            -1
#define NULL                0

typedef struct {
    int t_sec,  //1/10 of second
        sec,    // second
        min,    // minute
        hour,   // hour
        day,    // day
        month,  // month integer
        year; // year
    int alarm_cd; // alarm count down
} Systick_Clock; // Clock elements

void SYstickInit(); // initialization

#endif /* SYSTICK_H_ */

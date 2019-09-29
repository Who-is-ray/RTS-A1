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

#define MAX_T_SEC           9           // max value of 1/10 second
#define MAX_SEC             59          // max value of second
#define MAX_MIN             59          // max value of minute
#define MAX_HOUR            23          // max value of hour
#define NUM_OF_MON          12          // number of month
#define NUM_TYPE_OF_MON     2           // leap year month and normal year month
#define LEAP_YEAR_PERIOD    4           // leap year period
#define DISABLED            -1
#define NULL                0

typedef struct {
    int t_sec,  //1/10 of second
        sec,    // second
        min,    // minute
        hour,   // hour
        day,    // day
        month,  // month integer
        year;   // year
    int alarm_cd; // alarm count down
} Systick_Clock; // Clock elements

void SysTickInit(); // initialization
void Tick(); // clock tick, t_sec++
int IsDateVaild(int y/*year*/, int m/*month*/, int d/*day*/); //Check if date valid

#endif /* SYSTICK_H_ */
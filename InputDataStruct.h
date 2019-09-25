/*
 * InputDataStruct.h
 *
 *  Created on: Sep 24, 2019
 *      Author: Ray Su
 */

enum Source
{
    UART,
    SYSTICK
};

struct InputData
{
    enum Source source;
    char data;
};

/*
 * InputQueue.h
 *
 *  Created on: Sep 24, 2019
 *      Author: Ray SU
 */

#include "InputDataStruct.h"

#define QueueSize 8

struct InputQueue
{
    char Head;
    char Tail;
    struct InputData* Queue[QueueSize];
};

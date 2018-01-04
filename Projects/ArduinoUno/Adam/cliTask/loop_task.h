#ifndef LOOP_TASK_H_INCLUDED
#define LOOP_TASK_H_INCLUDED

#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"

void vTaskLoop(void *arg);

#endif // LOOP_TASK_H_INCLUDED

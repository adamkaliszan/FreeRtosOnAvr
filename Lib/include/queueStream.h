#ifndef QUEUE_STREAM_H
#define QUEUE_STREAM_H

#include <inttypes.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "queue.h"

typedef struct
{
  xQueueHandle Rx;
  xQueueHandle Tx;
} streamBuffers_t;

void initQueueStream(FILE *stream, streamBuffers_t *buffer, xQueueHandle Rx, xQueueHandle Tx);

#endif

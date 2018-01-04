#ifndef VTY_H
#define VTY_H

#include "main.h"
#include <stdio.h>
#include <avr/pgmspace.h>

#include "memory_x.h"
#include "configuration.h"

#include "cmdline.h"


void VtyInit(cmdState_t *state, FILE *stream);

void printErrorInfo(cmdState_t *state);

void printStatus(FILE *stream);

enum errorType
{
  AllOK  = 0,
  noFile = 1,
  xModemFrameStartTimeout = 2,
  xModemByteSendTimeout = 3,
  xModemWrongFrameNo = 4,
  xModemFrameFrameNoCorrectionNotMatch = 5,
  xModemFrameCrc = 6,
  xModemRemoteSideCan = 7,
  xModemUnknownResponse = 8,
  noRemoteDevice = 9,
  bootloaderNotResponding = 10,
  cantOpenFile = 11
};

typedef enum errorType errorType_t;

#endif

#ifndef RC_IBUS_H
#define RC_IBUS_H

#include <stdint.h>

extern int8_t rcChannels[8];

uint8_t ibus_processDta(uint8_t dta);
void ibus_processMessage(void);

void ibus_newFrame(void);

uint16_t rc_getChan(uint8_t idx);
uint16_t rc_getNoOfNewFrames(void);
uint16_t rc_getNoOfCrcErrors(void);
uint16_t rc_getNoOfProcFrames(void);

#endif
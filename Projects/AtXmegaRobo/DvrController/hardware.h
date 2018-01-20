#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>
//#include <avr/interrupt.h>

//#include "hardwareConfig.h"
//#include "softwareConfig.h"

extern volatile int16_t adcResults[];


/**
 * Hardware initialize
 */

uint8_t ReadCalibrationByte(uint8_t index);
void hardwareInit(void);

void setOut  (uint8_t no);
void clearOut(uint8_t no);

void setOut1   (void);
void setOut2   (void);
void setOut3   (void);
void setOut4   (void);
void setOut5   (void);
void setOut6   (void);
void setOut7   (void);
void setOut8   (void);
void setOut9   (void);
void setOut10  (void);
void setOut11  (void);
void setOut12  (void);

void clearOut1 (void);
void clearOut2 (void);
void clearOut3 (void);
void clearOut4 (void);
void clearOut5 (void);
void clearOut6 (void);
void clearOut7 (void);
void clearOut8 (void);
void clearOut9 (void);
void clearOut10(void);
void clearOut11(void);
void clearOut12(void);


void HC12setAtMode(void);
void HC12setTransparentMode(void);


void readAdcResults(void);

int16_t getVoltagePwrSply(void);
int16_t getCurrentPwrSply(void);
int16_t getVoltageInternal(void);
int16_t getTemperatureInternal(void);

#endif


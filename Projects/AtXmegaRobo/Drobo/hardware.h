#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>
//#include <avr/interrupt.h>

//#include "hardwareConfig.h"
//#include "softwareConfig.h"




/**
 * Hardware initialize
 */

uint8_t ReadCalibrationByte(uint8_t index);
void hardwareInit(void);

void offHbridge(void);

void forwardA(uint8_t left, uint8_t right);
void backwordA(uint8_t left, uint8_t right);
void rotateLeftA(uint8_t left, uint8_t right);
void rotateRightA(uint8_t left, uint8_t right);

void forwardB(uint8_t left, uint8_t right);
void backwordB(uint8_t left, uint8_t right);
void rotateLeftB(uint8_t left, uint8_t right);
void rotateRightB(uint8_t left, uint8_t right);

void pwrOn4v3(void);
void pwrOff4v3(void);
void pwrOn3v3rpi(void);
void pwrOff3v3rpi(void);
void pwrOn4v3rpi(void);
void pwrOff4v3rpi(void);

uint8_t isPwr4v3(void);
uint8_t isPwr3v3rpi(void);
uint8_t isPwr4v3rpi(void);

#endif


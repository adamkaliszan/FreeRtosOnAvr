#ifndef HARDWARE_H
#define HARDWARE_H

#include <avr/io.h>
#include "main.h"

#define TxStart() (PORTD |=  0x0C)
#define TxStop()  (PORTD &=  0xF3)

extern uint8_t address;
extern char bHelloResp[];

/**
 * Herdware initialization
 */
void hardwareInit(void);

/**
 *  Switch on specyfied diode
 * @param ledNo - diode number (0-3)
 */
void ledOn(uint8_t ledNo);

/**
 *  Switch off specyfied diode
 * @param ledNo - diode number (0-3)
 */
void ledOff(uint8_t ledNo);

/**
 *  Toggle specyfied diode
 * @param ledNo - diode number (0-3)
 */
void ledToggle(uint8_t ledNo);

/**
 * Reads specyfied key state
 * @param keyNo - key number (0-3)
 * @return 0 - key pressed, > 0 key is not pressed
 */
char readKey(uint8_t keyNo);

/**
 * Switch on (enable) Led 1
 */
void led1on(void);

/**
 * Switch off (disable) Led 1
 */
void led1off(void);

/**
 * Toggle (change state) Led 1
 */
void led1toggle(void);

/**
 * Read key #1
 */
char readKey1(void);

/**
 * Switch on (enable) Led 2
 */
void led2on(void);

/**
 * Switch off (disable) Led 2
 */
void led2off(void);

/**
 * Toggle (change state) Led 2
 */
void led2toggle(void);

/**
 * Read key #2
 */
char readKey2(void);

/**
 * Switch on (enable) Led 3
 */
void led3on(void);

/**
 * Switch off (disable) Led 3
 */
void led3off(void);

/**
 * Toggle (change state) Led 3
 */
void led3toggle(void);

/**
 * Read key #3
 */
char readKey3(void);

/**
 * Switch on (enable) Led 4
 */
void led4on(void);

/**
 * Switch off (disable) Led 4
 */
void led4off(void);

/**
 * Toggle (change state) Led 4
 */
void led4toggle(void);

/**
 * Read key #4
 */
char readKey4(void);

#endif

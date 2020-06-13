#ifndef RC_TASK_H
#define RC_TASK_H

#include <FreeRTOS.h>

enum State {GET_LENGTH, GET_DATA, GET_CHKSUML, GET_CHKSUMH, DISCARD};

#define PROTOCOL_LENGTH           0x20 
#define PROTOCOL_OVERHEAD         3     /* packet is <len><cmd><data....><chkl><chkh>, overhead=cmd+chk bytes */
#define PROTOCOL_TIMEGAP          3     /* Packets are received very ~7ms so use ~half that for the gap */
#define PROTOCOL_CHANNELS         14
#define PROTOCOL_COMMAND40        0x40  /* Command to set servo or motor speed is always 0x40 */
#define PROTOCOL_COMMAND_DISCOVER 0x80  /* Command discover sensor (lowest 4 bits are sensor) */
#define PROTOCOL_COMMAND_TYPE     0x90  /* Command discover sensor (lowest 4 bits are sensor) */
#define PROTOCOL_COMMAND_VALUE    0xA0  /* Command send sensor data (lowest 4 bits are sensor) */
#define SENSORMAX                 10    /* Max number of sensors */

void vTaskRc(void *ptr);

#endif
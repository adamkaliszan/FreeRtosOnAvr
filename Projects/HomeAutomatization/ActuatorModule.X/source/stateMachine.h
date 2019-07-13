#ifndef AUTOMAT_H
#define AUTOMAT_H

#include <stdint.h>

typedef enum
{
  Activity_Iddle,
  Activity_UpFull,
  Activity_DownFull,
  Activity_Up,
  Activity_Down
} Activity_t;

typedef struct
{
  uint8_t keyUp_on;
  uint8_t keyDown_on;

  uint8_t keyUp_off;
  uint8_t keyDown_off;
  Activity_t activity;
}
StateKey_t;

uint8_t stateMachine_processKey(uint8_t klGoraOff, uint8_t klDolOff, StateKey_t *state);

#endif

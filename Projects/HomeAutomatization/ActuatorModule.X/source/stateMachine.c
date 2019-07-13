#include "stateMachine.h"

#define T_START       5
#define T_START_CONT  40
#define T_STOP        5

uint8_t stateMachine_processKey(uint8_t keyUpOff, uint8_t keyDownOff, StateKey_t *state)
{
  uint8_t wiadomosc = 0;

  if (keyUpOff)
  {
    if ((state->activity == Activity_Up) && (state->keyUp_off == T_STOP))
    {
      state->activity = Activity_Iddle;
      wiadomosc = 0x40;
    }
    state->keyUp_on = 0;
    state->keyUp_off++;
  }
  else
  {
    if (state->keyUp_on == T_START)
    {
      if (state->activity != Activity_Iddle)
      {
        state->activity = Activity_Iddle;
        wiadomosc = 0x40;
      }
      else
      {
        state->activity = Activity_UpFull;
        wiadomosc = 0xBF;
      }
    }
    if (state->keyUp_on == T_START_CONT)
    {
      state->activity = Activity_Up;
    }
    state->keyUp_on++;
    state->keyUp_off = 0;
  }

  if (keyDownOff)
  {
    if ((state->activity == Activity_Down) && (state->keyDown_off == T_STOP))
    {
      state->activity = Activity_Iddle;
      wiadomosc = 0x40;
    }
    state->keyDown_on = 0;
    state->keyDown_off++;
  }
  else
  {
    if (state->keyDown_on == T_START)
    {
      if (state->activity != Activity_Iddle)
      {
        state->activity = Activity_Iddle;
        wiadomosc = 0x40;
      }
      else
      {
        state->activity = Activity_DownFull;
        wiadomosc = 0x3F;
      }
    }
    if (state->keyDown_on == T_START_CONT)
    {
      state->activity = Activity_Down;
    }
    state->keyDown_on++;
    state->keyDown_off = 0;
  }
  
  if (state->keyDown_on == 255)
	  state->keyDown_on = 254;
  if (state->keyUp_on == 255)
	  state->keyUp_on = 254;
  
  if (state->keyDown_off == 255)
	  state->keyDown_off = 254;
  if (state->keyUp_off == 255)
	  state->keyUp_off = 254;

  return wiadomosc;
}

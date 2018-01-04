/**
 * @file        cmdline.h
 * @author      Pascal Stang, Adam Kaliszan
 * @brief       Command-Line Interface Library
 * @ingroup     protocols
 * @version     0.6
 * Created      2003.07.16
 * Revised      2010.04.23
 * Editor Tabs  2
 * Target MCU   Atmel AVR Series
 *
 * @par Description
 * This library provides cammand lineinterpreter, that works on many instances.
 * Each instance requires: separate input/output stream, and separate instance of cmdState struct
 * The library was optimised under memory consumption.
 *
 * @note: This code is currently below version 1.0, and therefore is considered
 * to be lacking in some functionality or documentation, or may not be fully
 * tested.  Nonetheless, you can expect most functions to work.
 *
 * This code is distributed under the GNU Public License
 * which can be found at http://www.gnu.org/licenses/gpl.txt
*/
//----- Include Files ---------------------------------------------------------

#include "main.h"

#include <stdio.h>          // fprint() support
#include <avr/io.h>         // include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>  // include interrupt support
#include <avr/pgmspace.h>   // include AVR program memory support
#include <string.h>         // include standard C string functions
#include <stdlib.h>         // include stdlib for string conversion functions

#include "cmdline.h"        // Configuration
#include "vt100.h"          // vty100 constans


// Constans Strings
const char CmdlinePromptNormal[]    PROGMEM  = "DomOs>";
const char CmdlinePromptEnable[]    PROGMEM  = "DomOs#";
const char CmdlinePromptConfigure[] PROGMEM  = "DomOs@";
const char CmdlineNotice[]          PROGMEM  = "cmdline: ";
const char CmdlineCmdNotFound[]     PROGMEM  = "# nk";


// internal commands
static void cmdlineRepaint            (cmdState_t *state, char *buf);
static void cmdlineDoHistory          (enum cliHistoryAction action, cmdState_t *state);
static void cmdlineProcessInputString (cmdState_t *state);
static void cmdlinePrintPrompt        (cmdState_t *state);
static void cmdlinePrintError         (cmdState_t *state);
//static void cmdStateClear             (cmdState_t *state);
static void cmdHistoryCopy            (cmdState_t *state);
static void cmdHistoryMove            (cmdState_t *state);


void cmdStateConfigure(cmdState_t * state, char *buffPtr, uint16_t bufferTotalSize, FILE *stream, const command_t *commands, enum cliModeState mode)
{
  memset(state, 0, sizeof(cmdState_t));
  memset(buffPtr, 0, bufferTotalSize);

  state->CmdlineBuffer = buffPtr;
  state->bufferMaxSize    = (uint8_t)(bufferTotalSize / CMD_STATE_HISTORY);

  state->cliMode = mode;
  state->cmdList = commands;

  uint8_t i;
  char *tmpPtr = buffPtr;
  for (i=0; i < CMD_STATE_HISTORY; i++)
  {
    state->CmdlineHistory[i] = tmpPtr;
    tmpPtr += state->bufferMaxSize;
  }
  state->myStdInOut = stream;
}

/*void cmdStateClear(cmdState_t *state)
{
  // reset vt100 processing state
  state->CmdlineInputVT100State = 0;

  // initialize input buffer
  state->CmdlineBufferLength = 0;
  state->CmdlineBufferEditPos = 0;

  // initialize executing function
  state->CmdlineExecFunction = 0;
}*/

void cmdlineInputFunc(char c, cmdState_t *state)
{
  uint8_t i;
  // process the received character

  // VT100 handling
  // are we processing a VT100 command?
  if(state->CmdlineInputVT100State == 2)
  {
    // we have already received ESC and [
    // now process the vt100 codeCmdlineExcBuffer
    switch(c)
    {
    case VT100_ARROWUP:
      cmdlineDoHistory(CMDLINE_HISTORY_PREV, state);
      break;
    case VT100_ARROWDOWN:
      cmdlineDoHistory(CMDLINE_HISTORY_NEXT, state);
      break;
    case VT100_ARROWRIGHT:
      if (state->bufferHistoryState == NOT_COPIED)
        cmdHistoryCopy(state);
      // if the edit position less than current string length
      if(state->CmdlineBufferEditPos < state->CmdlineBufferLength)
      {
        // increment the edit position
        state->CmdlineBufferEditPos++;
        // move cursor forward one space (no erase)
        fputc(ASCII_ESC        , state->myStdInOut);
        fputc('['              , state->myStdInOut);
        fputc(VT100_ARROWRIGHT , state->myStdInOut);
      }
      else
      {
        // else, ring the bell
        fputc(ASCII_BEL        , state->myStdInOut);
      }
      break;
    case VT100_ARROWLEFT:
      // if the edit position is non-zero
      if (state->bufferHistoryState == NOT_COPIED)
        cmdHistoryCopy(state);

      if(state->CmdlineBufferEditPos)
      {
        // decrement the edit position
        state->CmdlineBufferEditPos--;
        // move cursor back one space (no erase)
        fputc(ASCII_BS         , state->myStdInOut);
      }
      else
      {
        // else, ring the bell
        fputc(ASCII_BEL        , state->myStdInOut);
      }
      break;
    default:
      break;
    }
    // done, reset state
    state->CmdlineInputVT100State = 0;
    return;
  }
  else if(state->CmdlineInputVT100State == 1)
  {
    // we last received [ESC]
    if(c == '[')
    {
      state->CmdlineInputVT100State = 2;
      return;
    }
    else
      state->CmdlineInputVT100State = 0;
  }
  else
  {
    // anything else, reset state
    state->CmdlineInputVT100State = 0;
  }

  // Regular handling
  //Protection against buffer Overflow
  if (state->CmdlineBufferLength == state->bufferMaxSize)
  {
    state->CmdlineBufferLength--;
    for (i=1; i < state->bufferMaxSize; i++)
    {
      state->CmdlineBuffer[i-1] = state->CmdlineBuffer[i];
    }
  }

  if( (c >= 0x20) && (c < 0x7F) )
  {
    if (state->bufferHistoryState == NOT_COPIED)
      cmdHistoryCopy(state);
    // character is printable
    // is this a simple append
    if(state->CmdlineBufferEditPos == state->CmdlineBufferLength)
    {
      // echo character to the output
      fputc(c                , state->myStdInOut);
      // add it to the command line buffer
      state->CmdlineBuffer[state->CmdlineBufferEditPos++] = c;
      // update buffer length
      state->CmdlineBufferLength++;
    }
    else
    {
      // edit/cursor position != end of buffer
      // we're inserting characters at a mid-line edit position
      // make room at the insert point
      state->CmdlineBufferLength++;
      for(i=state->CmdlineBufferLength; i>state->CmdlineBufferEditPos; i--)
        state->CmdlineBuffer[i] = state->CmdlineBuffer[i-1];
      // insert character
      state->CmdlineBuffer[state->CmdlineBufferEditPos++] = c;
      // repaint
      cmdlineRepaint(state, state->CmdlineBuffer);
      // reposition cursor
      for(i=state->CmdlineBufferEditPos; i<state->CmdlineBufferLength; i++)
        fputc(ASCII_BS         , state->myStdInOut);
    }
  }
  // handle special characters
  else if(c == ASCII_CR)
  {
    if (state->bufferHistoryState == NOT_COPIED)
      cmdHistoryMove(state);

    // user pressed [ENTER]
    // echo CR and LF to terminal
    fputc(ASCII_CR         , state->myStdInOut);
    fputc(ASCII_LF         , state->myStdInOut);
    // add null termination to command
    state->CmdlineBuffer[state->CmdlineBufferLength++] = 0;
    state->CmdlineBufferEditPos++;
    // command is complete, process it
    cmdlineProcessInputString(state);
    // reset buffer
    state->CmdlineBufferLength = 0;
    state->CmdlineBufferEditPos = 0;
  }
  else if(c == ASCII_BS)
  {
    if(state->CmdlineBufferEditPos)
    {
      // is this a simple delete (off the end of the line)
      if(state->CmdlineBufferEditPos == state->CmdlineBufferLength)
      {
        // destructive backspace
        // echo backspace-space-backspace
        fputc(ASCII_BS         , state->myStdInOut);
        fputc(' '              , state->myStdInOut);
        fputc(ASCII_BS         , state->myStdInOut);
        // decrement our buffer length and edit position
        state->CmdlineBufferLength--;
        state->CmdlineBufferEditPos--;
      }
      else
      {
        // edit/cursor position != end of buffer
        // we're deleting characters at a mid-line edit position
        // shift characters down, effectively deleting
        state->CmdlineBufferLength--;
        state->CmdlineBufferEditPos--;
        for(i=state->CmdlineBufferEditPos; i<state->CmdlineBufferLength; i++)
          state->CmdlineBuffer[i] = state->CmdlineBuffer[i+1];
        // repaint
        cmdlineRepaint(state, state->CmdlineBuffer);
        // add space to clear leftover characters
        fputc(' '              , state->myStdInOut);
        // reposition cursor
        for(i=state->CmdlineBufferEditPos; i<(state->CmdlineBufferLength+1); i++)
          fputc(ASCII_BS       , state->myStdInOut);
      }
    }
    else
    {
      // else, ring the bell
      fputc(ASCII_BEL          , state->myStdInOut);
    }
  }
  else if(c == ASCII_DEL)
  {
    // not yet handled
  }
  else if(c == ASCII_ESC)
  {
    state->CmdlineInputVT100State = 1;
  }
}

void cmdlineRepaint(cmdState_t *state, char *buf)
{
  uint8_t i;

  // carriage return
  fputc(ASCII_CR         , state->myStdInOut);
  // print fresh prompt
  cmdlinePrintPrompt(state);
  // print the new command line buffer
  i = state->CmdlineBufferLength;
  while(i--)
    fputc(*buf++         , state->myStdInOut);
  i = state->bufferMaxSize - state->CmdlineBufferLength;
  while (i--)
    fputc(' ', state->myStdInOut);
  i = state->bufferMaxSize - state->CmdlineBufferLength;
  while (i--)
    fputc(ASCII_BS,  state->myStdInOut);
}

void cmdHistoryCopy(cmdState_t *state)
{
  if (state->historyDepthIdx != 0)
  {
    uint8_t historyReadIdx = (state->historyWrIdx - state->historyDepthIdx) & CMD_STATE_HISTORY_MASK;
    memset(state->CmdlineBuffer, 0, state->bufferMaxSize);
    strcpy(state->CmdlineBuffer, state->CmdlineHistory[historyReadIdx]);
  }

  state->historyDepthIdx = 0;
  state->bufferHistoryState = COPIED;
}

void cmdHistoryMove(cmdState_t *state)
{
  uint8_t i=state->historyDepthIdx;

  if (state->historyDepthIdx != 0)
  {
    state->CmdlineBuffer = state->CmdlineHistory[(state->historyWrIdx-i) & CMD_STATE_HISTORY_MASK];
    for ( ; i<CMD_STATE_HISTORY; i++)
    {
      state->CmdlineHistory[(state->historyWrIdx-i) & CMD_STATE_HISTORY_MASK] = state->CmdlineHistory[(state->historyWrIdx-i-1) & CMD_STATE_HISTORY_MASK];
    }
  }
  state->CmdlineHistory[state->historyWrIdx] = state->CmdlineBuffer;

  state->historyDepthIdx = 0;
  state->bufferHistoryState = COPIED;
}

void cmdlineDoHistory(enum cliHistoryAction action, cmdState_t *state)
{
  uint8_t historyReadIdx;
  switch(action)
  {
  case CMDLINE_HISTORY_SAVE:
    // copy CmdlineBuffer to history if not null string
    state->CmdlineBufferLength  = 0;
    state->CmdlineBufferEditPos = 0;
    state->bufferHistoryState   = NOT_COPIED;

    if( strlen(state->CmdlineBuffer) )
    {
      state->historyWrIdx++;
      state->historyWrIdx &= CMD_STATE_HISTORY_MASK;

      state->CmdlineBuffer = state->CmdlineHistory[state->historyWrIdx];
    }
    break;
  case CMDLINE_HISTORY_PREV:
    if (state->historyDepthIdx == CMD_STATE_HISTORY - 1)
      break;                                               //We are on the end of the history list

    historyReadIdx = (state->historyWrIdx - state->historyDepthIdx - 1) & CMD_STATE_HISTORY_MASK;

    if (state->CmdlineHistory[historyReadIdx][0] == 0)
      break;

    state->historyDepthIdx++;
    state->historyDepthIdx &= CMD_STATE_HISTORY_MASK;

    // set the buffer position to the end of the line
    state->CmdlineBufferLength = strlen(state->CmdlineHistory[historyReadIdx]);
    state->CmdlineBufferEditPos = state->CmdlineBufferLength;

    state->bufferHistoryState = NOT_COPIED;

    // "re-paint" line
    cmdlineRepaint(state, state->CmdlineHistory[historyReadIdx]);

    break;
  case CMDLINE_HISTORY_NEXT:
    if (state->historyDepthIdx == 0)
      break;                                               //We are on the begining of the history list

    state->historyDepthIdx --;
    historyReadIdx = (state->historyWrIdx - state->historyDepthIdx) & CMD_STATE_HISTORY_MASK;

    // set the buffer position to the end of the line
    state->CmdlineBufferLength = strlen(state->CmdlineHistory[historyReadIdx]);
    state->CmdlineBufferEditPos = state->CmdlineBufferLength;

    state->bufferHistoryState = NOT_COPIED;

    // "re-paint" line
    cmdlineRepaint(state, state->CmdlineHistory[historyReadIdx]);
    break;
  }
}

void cmdlineProcessInputString(cmdState_t *state)
{
  uint8_t i=0;
  state->CmdlineExcBuffer = state->CmdlineBuffer;                     // We will use exec buffer later to read the arguments

  while( !((state->CmdlineExcBuffer[i] == ' ')                        // find the end of the command (excluding arguments)
    || (state->CmdlineExcBuffer[i] == 0)) )                           // find first whitespace character in CmdlineBuffer
    i++;                                                              // i determines the cammand length

  if(!i)                                                              // command was null or empty
  {
    cmdlinePrintPrompt(state);                                        // output a new prompt
    return;
  }

  const command_t *tmpPtr = state->cmdList;                           // Set list of commands. The list depends of the cli mode
  command_t  tmp;                                                     // We need to create this object. We can't directly
  memcpy_P(&tmp, tmpPtr, sizeof(command_t));                          // read from flash. We need to copy it before.

  do                                                                  // search command list for match with entered command
  {
    if( !strncmp_P(state->CmdlineExcBuffer, tmp.commandStr, i) )      // user-entered command matched a command in the list
    {                                                                 //
      state->CmdlineExecFunction = tmp.commandFun;                    // set function pointer
      state->command_str         = tmp.commandStr;
      state->command_help_str    = tmp.commandHelpStr;
      cmdlineDoHistory(CMDLINE_HISTORY_SAVE, state);                  // save command in history
      return;
    }
    tmpPtr++;                                                         // Next command
    memcpy_P(&tmp, tmpPtr, sizeof(command_t));                        // Copy this command from flash to ram
  }
  while (tmp.commandStr != NULL);                                     // Last command on the list is NULL. It is required !!!

  // if we did not get a match
  cmdlinePrintError(state);                                           // output an error message
  cmdlinePrintPrompt(state);                                          // output a new prompt
}

void cmdlineMainLoop(cmdState_t *state)
{
  cliExRes_t result;
  if(state->CmdlineExecFunction)                // do we have a command/function to be executed
  {
    state->argc = cmdLineGetLastArgIdx(state);  // get number of arguments
    result = state->CmdlineExecFunction(state); // run it

    switch(result)
    {
      case OK_INFORM:
        fprintf_P(state->myStdInOut, PSTR("OK\r\n"));
        break;
      case SYNTAX_ERROR:
        fprintf_P(state->myStdInOut, PSTR("Syntax Error. Use: "));
        fprintf_P(state->myStdInOut, state->command_str);
        fprintf_P(state->myStdInOut, PSTR(" "));
        fprintf_P(state->myStdInOut, state->command_help_str);
        fprintf_P(state->myStdInOut, PSTR("\r\n"));
        break;
      case ERROR_INFORM:
        fprintf_P(state->myStdInOut, PSTR("Operation failed\r\n"));
        break;
      case ERROR_OPERATION_NOT_ALLOWED:
        fprintf_P(state->myStdInOut, PSTR("Operation not allowed\r\n"));
        break;
      default:
        break;
    }
    state->CmdlineExecFunction = NULL;          // reset
    state->command_str         = NULL;
    state->command_help_str    = NULL;
    cmdlinePrintPrompt(state);                  // output new prompt
  }
}

void cmdlinePrintPrompt(cmdState_t *state)
{
  const char* ptr;
  // print a new command prompt
  switch (state->cliMode)
  {
    case NR_NORMAL:
      ptr = CmdlinePromptNormal;
      break;
    case NR_ENABLE:
      ptr = CmdlinePromptEnable;
      break;
    case NR_CONFIGURE:
      ptr = CmdlinePromptConfigure;
      break;
    default:
      ptr = CmdlinePromptNormal;
      break;
  }
  while(pgm_read_byte(ptr))
    fputc(pgm_read_byte(ptr++)    , state->myStdInOut);
}

void cmdlinePrintError(cmdState_t *state)
{
  char * ptr;

  // print a notice header
  // (uint8_t*) cast used to avoid compiler warning
  ptr = (char*)CmdlineNotice;
  while(pgm_read_byte(ptr))
    fputc(pgm_read_byte(ptr++)    , state->myStdInOut);

  // print the offending command
  ptr = state->CmdlineBuffer;
  while((*ptr) && (*ptr != ' '))
    fputc(*ptr++    , state->myStdInOut);

  fputc(':'         , state->myStdInOut);
  fputc(' '         , state->myStdInOut);

  // print the not-found message
  // (uint8_t*) cast used to avoid compiler warning
  ptr = (char*)CmdlineCmdNotFound;
  while(pgm_read_byte(ptr))
    fputc(pgm_read_byte(ptr++)    , state->myStdInOut);

  fputc('\r'        , state->myStdInOut);
  fputc('\n'        , state->myStdInOut);
}


uint8_t cmdLineGetLastArgIdx(cmdState_t *state)
{
  uint8_t result = 0;
  uint8_t lastWhite = 1;
  char *str = state->CmdlineExcBuffer;
  while(*str != 0)
  {
    if (*str == ' ')
    {
      if (lastWhite == 0)
        result++;
      lastWhite = 1;
    }
    else
      lastWhite = 0;
    str++;
  }
  return result;
}

char* cmdlineGetArgStr(uint8_t argnum, cmdState_t *state)
{
  // find the offset of argument number [argnum]
  uint8_t idx=0;
  uint8_t arg;

  // find the first non-whitespace character
  while( (state->CmdlineExcBuffer[idx] != 0) && (state->CmdlineExcBuffer[idx] == ' ')) idx++;

  // we are at the first argument
  for(arg=0; arg<argnum; arg++)
  {
    // find the next whitespace character
    while( (state->CmdlineExcBuffer[idx] != 0) && (state->CmdlineExcBuffer[idx] != ' ')) idx++;
    // find the first non-whitespace character
    while( (state->CmdlineExcBuffer[idx] != 0) && (state->CmdlineExcBuffer[idx] == ' ')) idx++;
  }
  // we are at the requested argument or the end of the buffer
  return &state->CmdlineExcBuffer[idx];
}

// return argument [argnum] interpreted as a decimal integer
long cmdlineGetArgInt(uint8_t argnum, cmdState_t *state)
{
  char* endptr;
  return strtol(cmdlineGetArgStr(argnum, state), &endptr, 10);
}

// return argument [argnum] interpreted as a hex integer
long cmdlineGetArgHex(uint8_t argnum, cmdState_t *state)
{
  char* endptr;
  return strtol(cmdlineGetArgStr(argnum, state), &endptr, 16);
}

void cmdPrintHelp(cmdState_t *state)
{
  command_t  tmp;
  const command_t *tmpPtr = state->cmdList;

  memcpy_P(&tmp, tmpPtr, sizeof(command_t));
  do
  {
    fprintf_P(state->myStdInOut, tmp.commandStr);
    fprintf_P(state->myStdInOut, PSTR("\t"));
    fprintf_P(state->myStdInOut, tmp.commandHelpStr);
    fprintf_P(state->myStdInOut, PSTR("\r\n"));

    tmpPtr++;
    memcpy_P(&tmp, tmpPtr, sizeof(command_t));
  }
  while (tmp.commandFun != NULL);
}

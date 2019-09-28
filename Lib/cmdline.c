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

#include <stdio.h>          // fprint() support
#include <avr/io.h>         // include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>  // include interrupt support
#if USE_XC8
#undef PROGMEM
#define PROGMEM
#else
#include <avr/pgmspace.h>   // include AVR program memory support
#endif
#include <string.h>         // include standard C string functions
#include <stdlib.h>         // include stdlib for string conversion functions

#include "cmdline.h"        // Configuration
#include "vt100.h"          // vty100 constans


// Constans Strings
const char cmdlinePromptNormal[]    PROGMEM  = "DomOs>";
const char cmdlinePromptEnable[]    PROGMEM  = "DomOs#";
const char cmdlinePromptConfigure[] PROGMEM  = "DomOs@";
const char cmdlineNotice[]          PROGMEM  = "cmdline: ";
const char cmdlineCmdNotFound[]     PROGMEM  = "# nk";


// internal commands
static void cmdlineRepaint            (CmdState_t *state, char *buf);
static void cmdlineDoHistory          (enum CliHistoryAction action, CmdState_t *state);
static void cmdlineProcessInputString (CmdState_t *state);
static void cmdlinePrintPrompt        (CmdState_t *state);
static void cmdlinePrintError         (CmdState_t *state);
//static void cmdStateClear             (CmdState_t *state);
static void cmdHistoryCopy            (CmdState_t *state);
static void cmdHistoryMove            (CmdState_t *state);

static uint8_t hexToInt(uint8_t hex)
{
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    
    if (hex >= 'a' && hex <= 'f')
        return hex - 'a' + 10;

    if (hex >= 'A' && hex <= 'F')
        return hex - 'A' + 10;

    return 0;
}

uint8_t hexStrToDataN(uint8_t *data, const uint8_t *hexStr, uint8_t maxLen)
{
    uint8_t result = 0;
    const uint8_t *srcPntr = hexStr;
    uint8_t dataA;
    uint8_t dataB;
    while (result < maxLen)
    {
        if (*srcPntr == '\0')
            break;

        if (*(srcPntr+1) == '\0')
            break;

        dataA = *(srcPntr++);
        
        if ((dataA == ' ') || (dataA == 'X') || (dataA == 'x'))
            continue;
        
        dataB = *(srcPntr++);

        if ((dataB == ' ') || (dataB == 'X') || (dataB == 'x'))
            continue;

        *(data++) = (hexToInt(dataA) << 4) + hexToInt(dataB);
        
        result++;
    }
    
    return result;
}



void cmdStateConfigure(CmdState_t * state, char *buffPtr, uint16_t bufferTotalSize, FILE *stream, const Command_t *commands, enum CliModeState mode)
{
  memset(state, 0, sizeof(CmdState_t));
  memset(buffPtr, 0, bufferTotalSize);

  state->cmdlineBuffer = buffPtr;
  state->bufferMaxSize    = (uint8_t)(bufferTotalSize / CMD_STATE_HISTORY);

  state->cliMode = mode;
  state->cmdList = commands;

  uint8_t i;
  char *tmpPtr = buffPtr;
  for (i=0; i < CMD_STATE_HISTORY; i++)
  {
    state->cmdlineHistory[i] = tmpPtr;
    tmpPtr += state->bufferMaxSize;
  }
  state->myStdInOut = stream;
}

/*void cmdStateClear(CmdState_t *state)
{
  // reset vt100 processing state
  state->CmdlineInputVT100State = 0;

  // initialize input buffer
  state->CmdlineBufferLength = 0;
  state->CmdlineBufferEditPos = 0;

  // initialize executing function
  state->CmdlineExecFunction = 0;
}*/

void cmdlineInputFunc(char c, CmdState_t *state)
{
  uint8_t i;
  // process the received character

  // VT100 handling
  // are we processing a VT100 command?
  if(state->cmdlineInputVT100State == 2)
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
      if(state->cmdlineBufferEditPos < state->cmdlineBufferLength)
      {
        // increment the edit position
        state->cmdlineBufferEditPos++;
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

      if(state->cmdlineBufferEditPos)
      {
        // decrement the edit position
        state->cmdlineBufferEditPos--;
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
    state->cmdlineInputVT100State = 0;
    return;
  }
  else if(state->cmdlineInputVT100State == 1)
  {
    // we last received [ESC]
    if(c == '[')
    {
      state->cmdlineInputVT100State = 2;
      return;
    }
    else
      state->cmdlineInputVT100State = 0;
  }
  else
  {
    // anything else, reset state
    state->cmdlineInputVT100State = 0;
  }

  // Regular handling
  //Protection against buffer Overflow
  if (state->cmdlineBufferLength == state->bufferMaxSize)
  {
    state->cmdlineBufferLength--;
    for (i=1; i < state->bufferMaxSize; i++)
    {
      state->cmdlineBuffer[i-1] = state->cmdlineBuffer[i];
    }
  }

  if( (c >= 0x20) && (c < 0x7F) )
  {
    if (state->bufferHistoryState == NOT_COPIED)
      cmdHistoryCopy(state);
    // character is printable
    // is this a simple append
    if(state->cmdlineBufferEditPos == state->cmdlineBufferLength)
    {
      // echo character to the output
      fputc(c                , state->myStdInOut);
      // add it to the command line buffer
      state->cmdlineBuffer[state->cmdlineBufferEditPos++] = c;
      // update buffer length
      state->cmdlineBufferLength++;
    }
    else
    {
      // edit/cursor position != end of buffer
      // we're inserting characters at a mid-line edit position
      // make room at the insert point
      state->cmdlineBufferLength++;
      for(i=state->cmdlineBufferLength; i>state->cmdlineBufferEditPos; i--)
        state->cmdlineBuffer[i] = state->cmdlineBuffer[i-1];
      // insert character
      state->cmdlineBuffer[state->cmdlineBufferEditPos++] = c;
      // repaint
      cmdlineRepaint(state, state->cmdlineBuffer);
      // reposition cursor
      for(i=state->cmdlineBufferEditPos; i<state->cmdlineBufferLength; i++)
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
    state->cmdlineBuffer[state->cmdlineBufferLength++] = 0;
    state->cmdlineBufferEditPos++;
    // command is complete, process it
    cmdlineProcessInputString(state);
    // reset buffer
    state->cmdlineBufferLength = 0;
    state->cmdlineBufferEditPos = 0;
  }
  else if(c == ASCII_BS)
  {
    if(state->cmdlineBufferEditPos)
    {
      // is this a simple delete (off the end of the line)
      if(state->cmdlineBufferEditPos == state->cmdlineBufferLength)
      {
        // destructive backspace
        // echo backspace-space-backspace
        fputc(ASCII_BS         , state->myStdInOut);
        fputc(' '              , state->myStdInOut);
        fputc(ASCII_BS         , state->myStdInOut);
        // decrement our buffer length and edit position
        state->cmdlineBufferLength--;
        state->cmdlineBufferEditPos--;
      }
      else
      {
        // edit/cursor position != end of buffer
        // we're deleting characters at a mid-line edit position
        // shift characters down, effectively deleting
        state->cmdlineBufferLength--;
        state->cmdlineBufferEditPos--;
        for(i=state->cmdlineBufferEditPos; i<state->cmdlineBufferLength; i++)
          state->cmdlineBuffer[i] = state->cmdlineBuffer[i+1];
        // repaint
        cmdlineRepaint(state, state->cmdlineBuffer);
        // add space to clear leftover characters
        fputc(' '              , state->myStdInOut);
        // reposition cursor
        for(i=state->cmdlineBufferEditPos; i<(state->cmdlineBufferLength+1); i++)
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
    state->cmdlineInputVT100State = 1;
  }
}

void cmdlineRepaint(CmdState_t *state, char *buf)
{
  uint8_t i;

  // carriage return
  fputc(ASCII_CR         , state->myStdInOut);
  // print fresh prompt
  cmdlinePrintPrompt(state);
  // print the new command line buffer
  i = state->cmdlineBufferLength;
  while(i--)
    fputc(*buf++         , state->myStdInOut);
  i = state->bufferMaxSize - state->cmdlineBufferLength;
  while (i--)
    fputc(' ', state->myStdInOut);
  i = state->bufferMaxSize - state->cmdlineBufferLength;
  while (i--)
    fputc(ASCII_BS,  state->myStdInOut);
}

void cmdHistoryCopy(CmdState_t *state)
{
  if (state->historyDepthIdx != 0)
  {
    uint8_t historyReadIdx = (state->historyWrIdx - state->historyDepthIdx) & CMD_STATE_HISTORY_MASK;
    memset(state->cmdlineBuffer, 0, state->bufferMaxSize);
    strcpy(state->cmdlineBuffer, state->cmdlineHistory[historyReadIdx]);
  }

  state->historyDepthIdx = 0;
  state->bufferHistoryState = COPIED;
}

void cmdHistoryMove(CmdState_t *state)
{
  uint8_t i=state->historyDepthIdx;

  if (state->historyDepthIdx != 0)
  {
    state->cmdlineBuffer = state->cmdlineHistory[(state->historyWrIdx-i) & CMD_STATE_HISTORY_MASK];
    for ( ; i<CMD_STATE_HISTORY; i++)
    {
      state->cmdlineHistory[(state->historyWrIdx-i) & CMD_STATE_HISTORY_MASK] = state->cmdlineHistory[(state->historyWrIdx-i-1) & CMD_STATE_HISTORY_MASK];
    }
  }
  state->cmdlineHistory[state->historyWrIdx] = state->cmdlineBuffer;

  state->historyDepthIdx = 0;
  state->bufferHistoryState = COPIED;
}

void cmdlineDoHistory(enum CliHistoryAction action, CmdState_t *state)
{
  uint8_t historyReadIdx;
  switch(action)
  {
  case CMDLINE_HISTORY_SAVE:
    // copy CmdlineBuffer to history if not null string
    state->cmdlineBufferLength  = 0;
    state->cmdlineBufferEditPos = 0;
    state->bufferHistoryState   = NOT_COPIED;

    if( strlen(state->cmdlineBuffer) )
    {
      state->historyWrIdx++;
      state->historyWrIdx &= CMD_STATE_HISTORY_MASK;

      state->cmdlineBuffer = state->cmdlineHistory[state->historyWrIdx];
    }
    break;
  case CMDLINE_HISTORY_PREV:
    if (state->historyDepthIdx == CMD_STATE_HISTORY - 1)
      break;                                               //We are on the end of the history list

    historyReadIdx = (state->historyWrIdx - state->historyDepthIdx - 1) & CMD_STATE_HISTORY_MASK;

    if (state->cmdlineHistory[historyReadIdx][0] == 0)
      break;

    state->historyDepthIdx++;
    state->historyDepthIdx &= CMD_STATE_HISTORY_MASK;

    // set the buffer position to the end of the line
    state->cmdlineBufferLength = strlen(state->cmdlineHistory[historyReadIdx]);
    state->cmdlineBufferEditPos = state->cmdlineBufferLength;

    state->bufferHistoryState = NOT_COPIED;

    // "re-paint" line
    cmdlineRepaint(state, state->cmdlineHistory[historyReadIdx]);

    break;
  case CMDLINE_HISTORY_NEXT:
    if (state->historyDepthIdx == 0)
      break;                                               //We are on the begining of the history list

    state->historyDepthIdx --;
    historyReadIdx = (state->historyWrIdx - state->historyDepthIdx) & CMD_STATE_HISTORY_MASK;

    // set the buffer position to the end of the line
    state->cmdlineBufferLength = strlen(state->cmdlineHistory[historyReadIdx]);
    state->cmdlineBufferEditPos = state->cmdlineBufferLength;

    state->bufferHistoryState = NOT_COPIED;

    // "re-paint" line
    cmdlineRepaint(state, state->cmdlineHistory[historyReadIdx]);
    break;
  }
}

void cmdlineProcessInputString(CmdState_t *state)
{
  uint8_t i=0;
  state->cmdlineExcBuffer = state->cmdlineBuffer;                     // We will use exec buffer later to read the arguments

  while( !((state->cmdlineExcBuffer[i] == ' ')                        // find the end of the command (excluding arguments)
    || (state->cmdlineExcBuffer[i] == 0)) )                           // find first whitespace character in CmdlineBuffer
    i++;                                                              // i determines the cammand length

  if(!i)                                                              // command was null or empty
  {
    cmdlinePrintPrompt(state);                                        // output a new prompt
    return;
  }

#if USE_XC8

  const Command_t *tmpPtr = state->cmdList;                           // Set list of commands. The list depends of the cli mode
  
  do                                                                  // search command list for match with entered command
  {
    if( !strncmp(state->cmdlineExcBuffer, tmpPtr->commandStr, i) )      // user-entered command matched a command in the list
    {                                                                 //
      state->cmdlineExecFunction = tmpPtr->commandFun;                    // set function pointer
      state->command_str         = tmpPtr->commandStr;
      state->command_help_str    = tmpPtr->commandHelpStr;
      cmdlineDoHistory(CMDLINE_HISTORY_SAVE, state);                  // save command in history
      return;
    }
    tmpPtr++;                                                         // Next command
  }
  while (tmpPtr->commandStr != NULL);                                     // Last command on the list is NULL. It is required !!!
#else
  const Command_t *tmpPtr = state->cmdList;                           // Set list of commands. The list depends of the cli mode
  Command_t  tmp;                                                     // We need to create this object. We can't directly
  memcpy_P(&tmp, tmpPtr, sizeof(Command_t));                          // read from flash. We need to copy it before.

  do                                                                  // search command list for match with entered command
  {
    if( !strncmp_P(state->CmdlineExcBuffer, tmp.commandStr, i) )      // user-entered command matched a command in the list
    {                                                                 //
      state->cmdlineExecFunction = tmp.commandFun;                    // set function pointer
      state->command_str         = tmp.commandStr;
      state->command_help_str    = tmp.commandHelpStr;
      cmdlineDoHistory(CMDLINE_HISTORY_SAVE, state);                  // save command in history
      return;
    }
    tmpPtr++;                                                         // Next command
    memcpy_P(&tmp, tmpPtr, sizeof(Command_t));                        // Copy this command from flash to ram
  }
  while (tmp.commandStr != NULL); 

#endif

  // if we did not get a match
  cmdlinePrintError(state);                                           // output an error message
  cmdlinePrintPrompt(state);                                          // output a new prompt
}

void cmdlineMainLoop(CmdState_t *state)
{
  CliExRes_t result;
  if(state->cmdlineExecFunction)                // do we have a command/function to be executed
  {
    state->argc = cmdLineGetLastArgIdx(state);  // get number of arguments
    result = state->cmdlineExecFunction(state); // run it

    switch(result)
    {
      case OK_INFORM:
#if USE_XC8
        fprintf(state->myStdInOut, "OK\r\n");
#else
	fprintf_P(state->myStdInOut, PSTR("OK\r\n"));
#endif
        break;
      case SYNTAX_ERROR:
#if USE_XC8
        fprintf(state->myStdInOut, "Syntax Error. Use: ");
        fprintf(state->myStdInOut, state->command_str);
        fprintf(state->myStdInOut, " ");
        fprintf(state->myStdInOut, state->command_help_str);
        fprintf(state->myStdInOut, "\r\n");
#else
        fprintf_P(state->myStdInOut, PSTR("Syntax Error. Use: "));
        fprintf_P(state->myStdInOut, state->command_str);
        fprintf_P(state->myStdInOut, PSTR(" "));
        fprintf_P(state->myStdInOut, state->command_help_str);
        fprintf_P(state->myStdInOut, PSTR("\r\n"));
#endif
        break;
      case ERROR_INFORM:
#if USE_XC8
        fprintf(state->myStdInOut, "Operation failed\r\n");
#else
        fprintf_P(state->myStdInOut, PSTR("Operation failed\r\n"));
#endif
        break;
      case ERROR_OPERATION_NOT_ALLOWED:
#if USE_XC8
        fprintf(state->myStdInOut, "Operation not allowed\r\n");
#else
        fprintf_P(state->myStdInOut, PSTR("Operation not allowed\r\n"));
#endif
        break;
      default:
        break;
    }
    state->cmdlineExecFunction = NULL;          // reset
    state->command_str         = NULL;
    state->command_help_str    = NULL;
    cmdlinePrintPrompt(state);                  // output new prompt
  }
}

void cmdlinePrintPrompt(CmdState_t *state)
{
  const char* ptr;
  // print a new command prompt
  switch (state->cliMode)
  {
    case NR_NORMAL:
      ptr = cmdlinePromptNormal;
      break;
    case NR_ENABLE:
      ptr = cmdlinePromptEnable;
      break;
    case NR_CONFIGURE:
      ptr = cmdlinePromptConfigure;
      break;
    default:
      ptr = cmdlinePromptNormal;
      break;
  }
  while(pgm_read_byte(ptr))
    fputc(pgm_read_byte(ptr++)    , state->myStdInOut);
}

void cmdlinePrintError(CmdState_t *state)
{
#if USE_XC8
  const char * ptr;
#else
  char * ptr;
#endif
  // print a notice header
  // (uint8_t*) cast used to avoid compiler warning
  ptr = cmdlineNotice;
  while(pgm_read_byte(ptr))
    fputc(pgm_read_byte(ptr++)    , state->myStdInOut);

  // print the offending command
  ptr = state->cmdlineBuffer;
  while((*ptr) && (*ptr != ' '))
    fputc(*ptr++    , state->myStdInOut);

  fputc(':'         , state->myStdInOut);
  fputc(' '         , state->myStdInOut);

  // print the not-found message
  // (uint8_t*) cast used to avoid compiler warning
  ptr = cmdlineCmdNotFound;
#if USE_XC8
  while(*ptr)
  {
    fputc(*ptr    , state->myStdInOut);
    ptr++;
  }
#else
  while(pgm_read_byte(ptr))
    fputc(pgm_read_byte(ptr++)    , state->myStdInOut);
#endif
  fputc('\r'        , state->myStdInOut);
  fputc('\n'        , state->myStdInOut);
}


uint8_t cmdLineGetLastArgIdx(CmdState_t *state)
{
  uint8_t result = 0;
  uint8_t lastWhite = 1;
  char *str = state->cmdlineExcBuffer;
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

char* cmdlineGetArgStr(uint8_t argnum, CmdState_t *state)
{
  // find the offset of argument number [argnum]
  uint8_t idx=0;
  uint8_t arg;

  // find the first non-whitespace character
  while( (state->cmdlineExcBuffer[idx] != 0) && (state->cmdlineExcBuffer[idx] == ' ')) idx++;

  // we are at the first argument
  for(arg=0; arg<argnum; arg++)
  {
    // find the next whitespace character
    while( (state->cmdlineExcBuffer[idx] != 0) && (state->cmdlineExcBuffer[idx] != ' ')) idx++;
    // find the first non-whitespace character
    while( (state->cmdlineExcBuffer[idx] != 0) && (state->cmdlineExcBuffer[idx] == ' ')) idx++;
  }
  // we are at the requested argument or the end of the buffer
  return &state->cmdlineExcBuffer[idx];
}

// return argument [argnum] interpreted as a decimal integer
long cmdlineGetArgInt(uint8_t argnum, CmdState_t *state)
{
  const char* endptr;
  return strtol(cmdlineGetArgStr(argnum, state), &endptr, 10);
}

// return argument [argnum] interpreted as a hex integer
long cmdlineGetArgHex(uint8_t argnum, CmdState_t *state)
{
  const char* endptr;
  return strtol(cmdlineGetArgStr(argnum, state), &endptr, 16);
}

void cmdPrintHelp(CmdState_t *state)
{
#if USE_XC8
  const Command_t *tmpPtr = state->cmdList;
  do
  {
    fprintf(state->myStdInOut, tmpPtr->commandStr);
    fprintf(state->myStdInOut, "\t");
    fprintf(state->myStdInOut, tmpPtr->commandHelpStr);
    fprintf(state->myStdInOut, "\r\n");

    tmpPtr++;
  }
  while (tmpPtr->commandFun != NULL);
#else
  Command_t  tmp;
  const Command_t *tmpPtr = state->cmdList;

  memcpy_P(&tmp, tmpPtr, sizeof(Command_t));
  do
  {
    fprintf_P(state->myStdInOut, tmp.commandStr);
    fprintf_P(state->myStdInOut, PSTR("\t"));
    fprintf_P(state->myStdInOut, tmp.commandHelpStr);
    fprintf_P(state->myStdInOut, PSTR("\r\n"));

    tmpPtr++;
    memcpy_P(&tmp, tmpPtr, sizeof(Command_t));
  }
  while (tmp.commandFun != NULL);
#endif
}

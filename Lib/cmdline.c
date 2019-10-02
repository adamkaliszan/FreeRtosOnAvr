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
#include <stdlib.h>
#include <avr/pgmspace.h>         // include stdlib for string conversion functions

#include "cmdline.h"        // Configuration
#include "vt100.h"          // vty100 constans


// Constans Strings
const char cmdlinePromptNormal[]    PROGMEM  = "DomOs>";
const char cmdlinePromptEnable[]    PROGMEM  = "DomOs#";
const char cmdlinePromptConfigure[] PROGMEM  = "DomOs@";
const char cmdlineNotice[]          PROGMEM  = "cmdline: ";
const char cmdlineCmdNotFound[]     PROGMEM  = "# nk";


// internal commands
static void cliRepaint(CliState_t *state, char *buf);
static void cliHistoryMove(enum CliHistoryAction action, CliState_t *state);

static void cliInputDataParse(CliState_t *state);
static void cliInputDataProcess(CliState_t *state);

static void cliPrintPrompt(CliState_t *state);
static void cliPrintCommandNotFound(CliState_t *state);

static void cliHistoryLoad(CliState_t *state);

/**
 * argc and argv to array.
 * @param state
 */
static void cliUnparse(CliState_t *state);


static uint8_t cliHistorySave(CliState_t *state);


static inline void cliHistoryIncWrPointer(CliState_t *state);
static inline void cliHistoryIncRdPointerCharacter(CliState_t *state);
static inline void cliHistoryIncRdPointerItem(CliState_t *state);
static inline void cliHistoryDecRdPointerItem(CliState_t *state);



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



void cmdStateConfigure(CliState_t * state, FILE *stream, const Command_t *commands, enum CliModeState mode)
{
  memset(state, 0, sizeof(CliState_t));

  state->internalData.cliMode = mode;
  state->internalData.cmdList = commands;
 
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

#define CLI_ST_NEW_CMD        0x01
#define CLI_ST_SAVED          0x02
#define CLI_ST_PARSED         0x04

#define CLI_ST_HIST_LOADED    0x20
#define CLI_ST_HIST_SUGESTION 0x40
#define CLI_ST_HIST_ENABLE    0x80




void cmdlineInputFunc(char c, CliState_t *state)
{
    uint8_t i;
    // process the received character

/// VT100 handling
/// are we processing a VT100 command?
    if(state->internalData.vty100.cmdlineInputVT100State == 2)
    {
        // we have already received ESC and [. Process the vt100 codeCmdlineExcBuffer
        switch(c)
        {
        case VT100_ARROWUP:
            cliHistoryMove(CMDLINE_HISTORY_PREV, state);
            break;

        case VT100_ARROWDOWN:
            cliHistoryMove(CMDLINE_HISTORY_NEXT, state);
            break;
    
        case VT100_ARROWRIGHT:
            if(state->internalData.inputBuffer.editPos < state->internalData.inputBuffer.length)
            {
/// increment the edit position
                state->internalData.inputBuffer.editPos++;
/// move cursor forward one space (no erase)
                fputc(ASCII_ESC        , state->myStdInOut);
                fputc('['              , state->myStdInOut);
                fputc(VT100_ARROWRIGHT , state->myStdInOut);
            }
            else
            {
/// else, ring the bell
                fputc(ASCII_BEL        , state->myStdInOut);
            }
            break;

        case VT100_ARROWLEFT:
/// if the edit position is non-zero
            if(state->internalData.inputBuffer.editPos > 0)
            {
/// decrement the edit position
                state->internalData.inputBuffer.editPos--;
/// move cursor back one space (no erase)
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
/// done, reset state
        state->internalData.vty100.cmdlineInputVT100State = 0;
        return;
    }
    else if(state->internalData.vty100.cmdlineInputVT100State == 1)
    {
/// we last received [ESC]
        if(c == '[')
        {
            state->internalData.vty100.cmdlineInputVT100State = 2;
            return;
        }
        else
            state->internalData.vty100.cmdlineInputVT100State = 0;
    }
    else
    {
/// anything else, reset state
        state->internalData.vty100.cmdlineInputVT100State = 0;
    }

/// Regular handling
/// Protection against buffer Overflow
    if (state->internalData.inputBuffer.length == CLI_STATE_INP_CMD_LEN)
    {
        state->internalData.inputBuffer.length--;
        for (i=1; i < state->internalData.inputBuffer.length; i++)
        {
            state->internalData.inputBuffer.data[i-1] = state->internalData.inputBuffer.data[i];
        }
    }

    if( (c >= 0x20) && (c < 0x7F) )
    {
// character is printable
// is this a simple append
        if(state->internalData.inputBuffer.editPos == state->internalData.inputBuffer.length)
        {
/// echo character to the output
            fputc(c                , state->myStdInOut);
/// add it to the command line buffer
            state->internalData.inputBuffer.data[state->internalData.inputBuffer.editPos++] = c;
/// update buffer length
            state->internalData.inputBuffer.length++;
        }
        else
        {
/// edit/cursor position != end of buffer
/// we're inserting characters at a mid-line edit position
/// make room at the insert point
            state->internalData.inputBuffer.length++;
            for(i = state->internalData.inputBuffer.length; i > state->internalData.inputBuffer.editPos; i--)
                state->internalData.inputBuffer.data[i] = state->internalData.inputBuffer.data[i-1];
/// insert character
            state->internalData.inputBuffer.data[state->internalData.inputBuffer.editPos++] = c;
/// repaint
            cliRepaint(state, state->internalData.inputBuffer.data);
/// reposition cursor
            for(i=state->internalData.inputBuffer.editPos; i < state->internalData.inputBuffer.length; i++)
                fputc(ASCII_BS         , state->myStdInOut);
        }
    }
/// handle special characters
    else if(c == ASCII_CR)
    {
/// user pressed [ENTER]
/// echo CR and LF to terminal
        fputc(ASCII_CR         , state->myStdInOut);
        fputc(ASCII_LF         , state->myStdInOut);
/// add null termination to command
        state->internalData.inputBuffer.data[state->internalData.inputBuffer.length++] = 0;
        state->internalData.inputBuffer.editPos++;
/// command is complete, process it
        cliInputDataProcess(state);
    }
    else if(c == ASCII_BS)
    {
        if(state->internalData.inputBuffer.editPos)
        {
/// is this a simple delete (off the end of the line)
            if(state->internalData.inputBuffer.editPos == state->internalData.inputBuffer.length)
            {
/// destructive backspace
/// echo backspace-space-backspace
                fputc(ASCII_BS         , state->myStdInOut);
                fputc(' '              , state->myStdInOut);
                fputc(ASCII_BS         , state->myStdInOut);
/// decrement our buffer length and edit position
                state->internalData.inputBuffer.length--;
                state->internalData.inputBuffer.editPos--;
            }
            else
            {
/// edit/cursor position != end of buffer
/// we're deleting characters at a mid-line edit position
/// shift characters down, effectively deleting
                state->internalData.inputBuffer.length--;
                state->internalData.inputBuffer.editPos--;
                for(i = state->internalData.inputBuffer.editPos; i < state->internalData.inputBuffer.length; i++)
                    state->internalData.inputBuffer.data[i] = state->internalData.inputBuffer.data[i+1];
/// repaint
                cliRepaint(state, state->internalData.inputBuffer.data);
/// add space to clear leftover characters
                fputc(' '              , state->myStdInOut);
/// reposition cursor
                for(i = state->internalData.inputBuffer.editPos; i < (state->internalData.inputBuffer.length+1); i++)
                    fputc(ASCII_BS       , state->myStdInOut);
            }
        }
        else
        {
/// else, ring the bell
            fputc(ASCII_BEL          , state->myStdInOut);
        }
    }
    else if(c == ASCII_DEL)
    {
/// not yet handled
    }
    else if(c == ASCII_ESC)
    {
        state->internalData.vty100.cmdlineInputVT100State = 1;
    }
}

void cliRepaint(CliState_t *state, char *buf)
{
    uint8_t i;

/// carriage return
    fputc(ASCII_CR         , state->myStdInOut);
/// print fresh prompt
    cliPrintPrompt(state);
/// print the new command line buffer
    i = state->internalData.inputBuffer.length;
    while(i--)
        fputc(*buf++         , state->myStdInOut);

    i = CLI_STATE_INP_CMD_LEN - state->internalData.inputBuffer.length;
    while (i--)
        fputc(' ', state->myStdInOut);

    i = CLI_STATE_INP_CMD_LEN - state->internalData.inputBuffer.length;
    while (i--)
        fputc(ASCII_BS,  state->myStdInOut);
}

void cliHistoryMove(enum CliHistoryAction action, CliState_t *state)
{
#if CLI_STATE_HISTORY_LEN <= 256
    uint8_t cntr = 0;
#else
    uint16_t cntr = 0;
#endif
    
    if (action == CMDLINE_HISTORY_PREV)
    {
        while (state->internalData.history.data[state->internalData.history.rdIdx] != '\0')
        {
            cliHistoryDecRdPointerItem(state);
            cntr++;
            if (cntr > CLI_STATE_HISTORY_LEN)
                return;
        }

        while (state->internalData.history.data[state->internalData.history.rdIdx] == '\0')
        {
            cliHistoryDecRdPointerItem(state);
            cntr++;
            if (cntr > CLI_STATE_HISTORY_LEN)
                return;
        }
        
    }
    else
    {
        while (state->internalData.history.data[state->internalData.history.rdIdx] == '\0')
        {
            cliHistoryIncWrPointer(state);
            cntr++;
            if (cntr > CLI_STATE_HISTORY_LEN)
                return;
        }

        while (state->internalData.history.data[state->internalData.history.rdIdx] != '\0')
        {
            cliHistoryIncWrPointer(state);
            cntr++;
            if (cntr > CLI_STATE_HISTORY_LEN)
                return;
        }        
    }
    cliHistoryLoad(state);
}

void cliInputDataProcess(CliState_t *state)
{
    uint8_t firstArgLen=0;
    const Command_t *tmpPtr = state->internalData.cmdList;                // Set list of commands. The list depends of the cli mode

    while( !((state->internalData.inputBuffer.data[firstArgLen] == ' ')             // find the end of the command (excluding arguments)
    || (state->internalData.inputBuffer.data[firstArgLen] == 0)) )                  // find first whitespace character in CmdlineBuffer
        firstArgLen++;                                                              // i determines the cammand length

    if(firstArgLen > 0)                                                                // command was null or empty
    {
        do                                                                    // search command list for match with entered command
        {
#if USE_XC8
            if( !strncmp(state->internalData.inputBuffer.data, tmpPtr->commandStr, firstArgLen) )      // user-entered command matched a command in the list
            {
                memcpy(&state->internalData.cmd, tmpPtr, sizeof(Command_t));
#else
            memcpy_P(&state->internalData.cmd, tmpPtr, sizeof(Command_t));                  // read from flash. We need to copy it before
            if (state->internalData.cmd.commandStr == NULL)
                break;
            if( !strncmp(state->internalData.inputBuffer.data, tmp.commandStr, firstArgLen) )         // user-entered command matched a command in the list
            {                                                                 //
#endif
                if (state->internalData.cmd.maxArgC == 0)
                    state->internalData.cmd.maxArgC = CLI_STATE_MAX_ARGC;

                break;
#if USE_XC8
            }
            tmpPtr++;
#else
            }
            else
            {
                memset(&state->internalData.cmd, 0, sizeof(Command_t));
            }
#endif
        }
#if USE_XC8
        while (tmpPtr->commandStr != NULL);                                     // Last command on the list is NULL. It is required !!!
#else
        while (1);    
#endif
    }
    else
    {
        memset(&state->internalData.cmd, 0, sizeof(Command_t));
    }

    if (state->internalData.cmd.commandFun)
    {
        cliInputDataParse(state);    
    }
    else
    {
        if (firstArgLen > 0)
            cliPrintCommandNotFound(state);
        cliPrintPrompt(state);                                          // output a new prompt
    }
    state->internalData.inputBuffer.length = 0;
    state->internalData.inputBuffer.editPos = 0;
}


static void cliInputDataParse(CliState_t *state)
{
    char *data = state->internalData.inputBuffer.data;
    
    state->argv[0] = data;
    state->argc = 1;
    
    while (*data != '\0')
    {
        if (*data != ' ')
        {
            data++;
            continue;
        }
        
        *data = '\0';
        data++;
        
        if (*data == '\0')
            break;
        
        state->argv[state->argc] = data;        
        state->argc++;

        if ((state->argc == CLI_STATE_MAX_ARGC) || state->argc == state->internalData.cmd.maxArgC)
            break;
    }
}


static inline void cliHistoryIncRdPointerCharacter(CliState_t *state)
{
    if (++state->internalData.history.rdIdx >= CLI_STATE_HISTORY_LEN)
        state->internalData.history.rdIdx = 0;    
}

static inline void cliHistoryIncRdPointerItem(CliState_t *state)
{
    do
    {
        if (++state->internalData.history.rdIdx >= CLI_STATE_HISTORY_LEN)
            state->internalData.history.rdIdx = 0;
    }
    while(state->internalData.history.data[state->internalData.history.rdIdx] != '\0');

    do
    {
        if (++state->internalData.history.rdIdx >= CLI_STATE_HISTORY_LEN)
            state->internalData.history.rdIdx = 0;
    }
    while(state->internalData.history.data[state->internalData.history.rdIdx] == '\0');
}


static inline void cliHistoryDecRdPointerItem(CliState_t *state)
{
#if CLI_STATE_HISTORY_LEN <= 256
    uint8_t noOfMoves;
#else
    uint16_t noOfMoves;    
#endif
    noOfMoves = 0;
    
    state->internalData.history.rdIdx = (state->internalData.history.rdIdx == 0) ? 
        state->internalData.history.rdIdx = CLI_STATE_HISTORY_LEN -1 : 
        state->internalData.history.rdIdx -1;
   
    while (state->internalData.history.data[state->internalData.history.rdIdx] == '\0')
    {
        noOfMoves++;
                
        state->internalData.history.rdIdx = (state->internalData.history.rdIdx == 0) ? 
            state->internalData.history.rdIdx = CLI_STATE_HISTORY_LEN -1 : 
            state->internalData.history.rdIdx -1;
    }

    while (state->internalData.history.data[state->internalData.history.rdIdx] != '\0')
    {
        noOfMoves++;
        state->internalData.history.rdIdx = (state->internalData.history.rdIdx == 0) ? 
            state->internalData.history.rdIdx = CLI_STATE_HISTORY_LEN -1 : 
            state->internalData.history.rdIdx -1;

        if (noOfMoves == CLI_STATE_HISTORY_LEN)
            return; // It should never happen
    }

    state->internalData.history.rdIdx++;
    if (state->internalData.history.rdIdx >= CLI_STATE_HISTORY_LEN)
        state->internalData.history.rdIdx = 0;
}


static inline void cliHistoryIncWrPointer(CliState_t *state)
{
    state->internalData.history.wrIdx++;
    if (state->internalData.history.wrIdx >= CLI_STATE_HISTORY_LEN)
        state->internalData.history.wrIdx = 0;    
}


static void cliHistoryFreeIsNoSpace(CliState_t *state)
{
#if CLI_STATE_HISTORY_LEN > 255
    uint16_t tmpIdx = state->internalData.history.wrIdx;
#else
    uint8_t tmpIdx = state->internalData.history.wrIdx;    
#endif
    while (state->internalData.history.data[tmpIdx] != '\0')    // Check if the previous command need to be overwritten
    {
        state->internalData.history.data[tmpIdx] = '\0';
        tmpIdx++;
        if (tmpIdx == CLI_STATE_HISTORY_LEN)
            tmpIdx = 0;
    }   
}

static void cliHistoryDontSave(CliState_t *state)
{
    state->internalData.state &= ~CLI_ST_SAVED; 
}

static uint8_t cliHistorySave(CliState_t *state)
{
    uint8_t i;
    uint8_t result = 0;
        
    state->internalData.history.rdIdx = state->internalData.history.wrIdx;
    
    const char *src;
    
    for (i=0; i<state->argc; i++)
    {
        src = state->argv[i];
        while(*src != '\0')
        {
            cliHistoryFreeIsNoSpace(state);
                
            state->internalData.history.data[state->internalData.history.wrIdx] = *src;
            cliHistoryIncWrPointer(state);
            
            src++;
            result++;
        }
        cliHistoryFreeIsNoSpace(state);
                
        state->internalData.history.data[state->internalData.history.wrIdx] = (i+1 == state->argc) ? '\0' : ' ';
        cliHistoryIncWrPointer(state);

        result++;            
    }    
    return result;
}

static void cliHistoryLoad(CliState_t *state)
{
    char *dst = state->internalData.inputBuffer.data;

    memset(state->internalData.inputBuffer.data, 0, CLI_STATE_INP_CMD_LEN);
    state->internalData.inputBuffer.length = 0;
    state->internalData.inputBuffer.editPos = 0;
    
    if ((state->internalData.state & CLI_ST_HIST_ENABLE) != 0)
    {
        state->internalData.state |= CLI_ST_HIST_LOADED;
        
        while (state->internalData.history.data[state->internalData.history.rdIdx] != '\0')
        {
            *dst = state->internalData.history.data[state->internalData.history.rdIdx];
            cliHistoryIncWrPointer(state);
            
            state->internalData.inputBuffer.length++;

            if (state->internalData.inputBuffer.length == CLI_STATE_INP_CMD_LEN)
            {
                *dst = '\0';
                break;
            }            
            dst++;
        } 
    }
}

static void cliUnparse(CliState_t *state)
{
    uint8_t i;
    const char *src;
    char *dst = state->internalData.inputBuffer.data;

    for (i=0; i<state->argc; i++)
    {
        src = state->argv[i];
        while(*src != '\0')
        {                
            *dst = *src;
            src++;
            state->internalData.inputBuffer.length++;
                
            if (state->internalData.inputBuffer.length == CLI_STATE_INP_CMD_LEN)
            {
                *dst = '\0';
                break;
            }
            dst++;
        }
    }
    *dst = ((i+1 == state->argc) || (state->internalData.inputBuffer.length == CLI_STATE_INP_CMD_LEN)) ? '\0' : ' ';
}

void cliMainLoop(CliState_t *state)
{
    CliExRes_t result;
    if(state->internalData.cmd.commandFun)                // do we have a command/function to be executed
    {
        result = state->internalData.cmd.commandFun(state); // run it

        switch(result)
        {
        case OK_INFORM:
#if USE_XC8
            fprintf(state->myStdInOut, "OK\r\n");
#else
            fprintf_P(state->myStdInOut, PSTR("OK\r\n"));
#endif

        case OK_SILENT:
            cliHistorySave(state);

            break;
            
        case SYNTAX_ERROR:
            cliHistoryDontSave(state);
#if USE_XC8
            fprintf(state->myStdInOut, "Syntax Error. Use: ");
            fprintf(state->myStdInOut, state->internalData.cmd.commandStr);
            fprintf(state->myStdInOut, " ");
            fprintf(state->myStdInOut, state->internalData.cmd.commandHelpStr);
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
            cliHistoryDontSave(state);
#if USE_XC8
            fprintf(state->myStdInOut, "Operation failed\r\n");
#else
            fprintf_P(state->myStdInOut, PSTR("Operation failed\r\n"));
#endif
            break;
            
        case ERROR_OPERATION_NOT_ALLOWED:
            cliHistoryDontSave(state);
#if USE_XC8
            fprintf(state->myStdInOut, "Operation not allowed\r\n");
#else
            fprintf_P(state->myStdInOut, PSTR("Operation not allowed\r\n"));
#endif
            break;

        default:
            cliHistoryDontSave(state);
            break;
        }
/// reset
        state->internalData.cmd.commandFun     = NULL;
        state->internalData.cmd.commandStr     = NULL;
        state->internalData.cmd.commandHelpStr = NULL;
/// output new prompt
        cliPrintPrompt(state);                  
    }
}

void cliPrintPrompt(CliState_t *state)
{
  const char* ptr;
  // print a new command prompt
  switch (state->internalData.cliMode)
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

void cliPrintCommandNotFound(CliState_t *state)
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
    ptr = state->internalData.inputBuffer.data;  //TODO Adam convert '\0' into ' '
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


void cmdPrintHistory(CliState_t *state)
{
    
#if CLI_STATE_HISTORY_LEN <= 256
//    uint8_t i;
    uint8_t rdIdxOld;
    uint8_t cmdOldIdx;
#else
//    uint16_t i;
    uint16_t rdIdxOld;
    uint16_t cmdOldIdx;
#endif

    rdIdxOld = state->internalData.history.rdIdx;

    
/*
#if USE_XC8
    uint8_t lastZero = 0;
    fprintf(state->myStdInOut, "History bufer");
    for (i=0; i< CLI_STATE_HISTORY_LEN; i++)
    {
        if (i %32 == 0)
            fprintf(state->myStdInOut, "\r\n");
        if (state->internalData.history.data[i] == '\0')
        {
            if (!lastZero)
                fputc('~', state->myStdInOut);
            lastZero = 1;
            continue;
        }
        else
            lastZero = 0;
        fputc(state->internalData.history.data[i], state->myStdInOut);
    }
    fprintf(state->myStdInOut, "\r\nBuffer rd idx = %d\r\n", state->internalData.history.rdIdx);
    fprintf(state->myStdInOut,    "        wr idx = %d\r\n", state->internalData.history.wrIdx);
#else
    fprintf_P(state->myStdInOut, PSTR("History\r\n"));    
#endif
*/
    int tmp = 0;
    do
    {
        cmdOldIdx = state->internalData.history.rdIdx;
//        fprintf(state->myStdInOut, "\t Idx = %d\n", state->internalData.history.rdIdx);
        fputc('\t', state->myStdInOut);
        while (state->internalData.history.data[state->internalData.history.rdIdx] != '\0')
        {
            fputc(state->internalData.history.data[state->internalData.history.rdIdx], state->myStdInOut);
            cliHistoryIncRdPointerCharacter(state);
            tmp++;
            if (tmp >= CLI_STATE_HISTORY_LEN)
                break;
        }
        fputc('\r', state->myStdInOut);
        fputc('\n', state->myStdInOut);
                 
        state->internalData.history.rdIdx = cmdOldIdx;
        cliHistoryDecRdPointerItem(state);
        
        tmp++;
            if (tmp >= CLI_STATE_HISTORY_LEN)
                break;
    }
    while (rdIdxOld != state->internalData.history.rdIdx);
    
    state->internalData.history.rdIdx = rdIdxOld;
}

void cmdPrintHelp(CliState_t *state)
{
#if USE_XC8
     const Command_t *tmpPtr = state->internalData.cmdList;
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
    const Command_t *tmpPtr = state->internalData.cmdList;

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

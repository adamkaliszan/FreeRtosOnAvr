/**
 * @file        cmdline.h
 * @author      Adam Kaliszan adam.kaliszan@gmail.com
 * @brief       Command-Line Interface Library
 * @ingroup     protocols
 * @version     0.9
 * Created      ???
 * Revised      2020.05.17
 * Editor Tabs  2
 * Target MCU   Atmel AVR Series xc8 and avrgcc compiler
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h> 
#include <stdint.h>

#include <avr/io.h>         // include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>  // include interrupt support

#if USE_XC8
#undef PROGMEM
#define PROGMEM
#else
#include <avr/pgmspace.h>   // include AVR program memory support
#endif

#include "softwareConfig.h"
#include "cmdline.h"        // Configuration
#include "vt100.h"          // vty100 constans




// Constans Strings
const char cmdlinePromptNormal[]    PROGMEM  = "DomOs>";
const char cmdlinePromptEnable[]    PROGMEM  = "DomOs#";
const char cmdlinePromptConfigure[] PROGMEM  = "DomOs@";
const char cmdlineNotice[]          PROGMEM  = "cmdline: ";
const char cmdlineCmdNotFound[]     PROGMEM  = "# nk";


static uint8_t hexToInt(uint8_t hex);
inline static void vt100procCmd(char c, CliState_t *state);


// internal commands

static void cliHistorySave(CliState_t *state);
static void cliRepaint(CliState_t *state);

static void cliHistoryNavigateTop(CliState_t *state);
static void cliHistoryNavigateYounger(CliState_t *state);
static void cliHistoryLoadAndMovePointer(CliState_t *state);

static void cliInputDataParse(CliState_t *state);
static void cliUnparse(CliState_t *state);
static void cliInputDataProcess(CliState_t *state);

static void cliPrintPrompt(CliState_t *state);
static void cliPrintCommandNotFound(CliState_t *state);

//static void cliHistoryLoad(CliState_t *state);

/**
 * argc and argv to array.
 * @param state
 */
static void cliUnparse(CliState_t *state);


void cliMainLoop(CliState_t *state)
{
    CliExRes_t result;
    if(state->internalData.cmd.commandFun)                // do we have a command/function to be executed
    {
        result = state->internalData.cmd.commandFun(state); // run it

        switch(result)
        {
        case OK_INFORM:
            CMD_msg("OK\r\n");
        case OK_SILENT:
            cliHistorySave(state);
            break;
            
        case SYNTAX_ERROR:
            CMD_msg("Syntax Error. Use: ");
            CMD_msg_const(state->internalData.cmd.commandStr);
            CMD_msg(" ");
            CMD_msg_const(state->internalData.cmd.commandHelpStr);
            CMD_msg("\r\n");
            break;
            
        case ERROR_INFORM:
            CMD_msg("Operation failed\r\n");

            break;
            
        case ERROR_OPERATION_NOT_ALLOWED:
            CMD_msg("Operation not allowed\r\n");
            break;

        default:
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

static void cliHistoryRemoveOldest(CliState_t *state);

void cmdStateConfigure(CliState_t * state, FILE *stream, const Command_t *commands, enum CliModeState mode)
{
  memset(state, 0, sizeof(CliState_t));

  state->internalData.cliMode = mode;
  state->internalData.cmdList = commands;
 
  state->myStdInOut = stream;
  
  state->internalData.buffer.history.rdIdx = CLI_STATE_BUF_LEN - 1;
  state->internalData.buffer.history.wrIdx = CLI_STATE_BUF_LEN - 1;  
}


/**
 * Process Vt100 protocol command
 * We have already received ESC and [. Process the vt100 codeCmdlineExcBuffer
 * @param c received character
 * @param state Cli instance
 */
inline static void vt100procCmd(char c, CliState_t *state)
{
    switch(c)
    {
    case VT100_ARROWUP:
        switch (state->internalData.state)
        {
        case CLI_ST_EMPTY:
        case CLI_ST_READING_CMD:
            if (state->internalData.buffer.history.depthLength > 0)
            {
                cliHistoryNavigateTop(state);
                cliHistoryLoadAndMovePointer(state);
                state->internalData.state = CLI_ST_HISTORY;
                cliRepaint(state);
            }
            break;
        case CLI_ST_WRONG_CMD:
            if (state->argc > 0)
            {
                cliUnparse(state);
                cliRepaint(state);
                state->internalData.state = CLI_ST_READING_CMD;
            }
            break;
        case CLI_ST_HISTORY:
            cliHistoryLoadAndMovePointer(state);
            cliRepaint(state);
            break;
        }
        break;
    case VT100_ARROWDOWN:
        switch (state->internalData.state)
        {
        case CLI_ST_EMPTY:
        case CLI_ST_READING_CMD:
        case CLI_ST_WRONG_CMD:
            break;
        case CLI_ST_HISTORY:
            cliHistoryNavigateYounger(state);
            cliHistoryNavigateYounger(state);
            cliHistoryLoadAndMovePointer(state);
            cliRepaint(state);
            break;
        }
        break;

    case VT100_ARROWRIGHT:
        if ((state->internalData.state == CLI_ST_READING_CMD) && (state->internalData.buffer.input.length > state->internalData.buffer.input.editPos))
        {
            state->internalData.buffer.input.editPos++;
            cliRepaint(state);
        }
        else
        {
            fputc(ASCII_BEL, state->myStdInOut);
        }
        break;

    case VT100_ARROWLEFT:
        switch (state->internalData.state)
        {
        case CLI_ST_EMPTY:
        case CLI_ST_READING_CMD:
            break;

        case CLI_ST_HISTORY:
            state->internalData.state = CLI_ST_READING_CMD;
            break;

        case CLI_ST_WRONG_CMD:
            cliUnparse(state);
            state->internalData.state = CLI_ST_READING_CMD;
            break;
        }

        if (state->internalData.state == CLI_ST_READING_CMD)
        {
            if (state->internalData.buffer.input.editPos > 0)
            {
                state->internalData.buffer.input.editPos--;
                fputc(ASCII_BS, state->myStdInOut);
            }
            else
            {
                fputc(ASCII_BEL, state->myStdInOut);
            }
        }
        break;

    default:
        break;
    }
    state->internalData.vt100state = VT100_ST_DISABLED;    /// done, reset state
}

inline static void _cliInputBufferAppend(char c, CliState_t *state)
{
    state->internalData.buffer.data[state->internalData.buffer.input.editPos++] = c;        /// add it to the command line buffer
    state->internalData.buffer.input.length++;                                              /// update buffer length
}

inline static void _cliInputBufferInsert(char c, CliState_t *state)
{
#if CLI_STATE_INP_CMD_LEN > 255
    uint16_t i;
#else
    uint8_t i;
#endif
    state->internalData.buffer.input.length++;                                              /// update the buffer length
    for(i = state->internalData.buffer.input.length; i > state->internalData.buffer.input.editPos; i--)
    {
        state->internalData.buffer.data[i] = state->internalData.buffer.data[i-1];          /// Shift the data
    }
/// insert character
    state->internalData.buffer.data[state->internalData.buffer.input.editPos++] = c;        /// Insert the new character

}

inline static void _cliInputBufferClear(CliState_t *state)
{
    memset(state->internalData.buffer.data, 0, state->internalData.buffer.input.length);            
    state->internalData.buffer.input.length = 0;
    state->internalData.buffer.input.editPos = 0;    
}

inline static void cliAddInputData(char c, CliState_t *state)
{
    switch (state->internalData.state)
    {
    case CLI_ST_HISTORY:
        state->internalData.state = CLI_ST_READING_CMD;
        break;

    case CLI_ST_WRONG_CMD:
        state->internalData.state = CLI_ST_EMPTY;
        _cliInputBufferClear(state);
        break;

    case CLI_ST_EMPTY:
    case CLI_ST_READING_CMD:
        break;
    }

    if(state->internalData.buffer.input.editPos == state->internalData.buffer.input.length)
    {   
        _cliInputBufferAppend(c, state);            /// Add the new character on the end of line
        fputc(c, state->myStdInOut);                /// echo character to the output
    }
    else /// edit/cursor position != end of buffer
    {
        _cliInputBufferInsert(c, state);            /// insert the new char in the midde of line
        cliRepaint(state);                          /// Rewrite the existing CLI line
    }
    state->internalData.state = CLI_ST_READING_CMD;
}

inline static void cliAddInputBS(char c, CliState_t *state)
{
#if CLI_STATE_INP_CMD_LEN > 255
    uint16_t i;
#else
    uint8_t i;
#endif
    if(state->internalData.buffer.input.editPos > 0)
    {
        state->internalData.buffer.input.length--;
        state->internalData.buffer.input.editPos--;
        if(state->internalData.buffer.input.editPos != state->internalData.buffer.input.length)
        {
            for(i = state->internalData.buffer.input.editPos; i < state->internalData.buffer.input.length; i++)
            {
                state->internalData.buffer.data[i] = state->internalData.buffer.data[i+1];
            }
        }
        state->internalData.buffer.data[state->internalData.buffer.input.length] = '\0';
        if(state->internalData.buffer.input.editPos == state->internalData.buffer.input.length)
        {   // No need to use CLI repaint
            fputc(ASCII_BS         , state->myStdInOut);   // Move back
            fputc(' '              , state->myStdInOut);   // Clear and move next
            fputc(ASCII_BS         , state->myStdInOut);   // Move back again
        }
        else
        {
            cliRepaint(state);
        }
    }
    else
    {
        fputc(ASCII_BEL          , state->myStdInOut);
    }
}

inline static void cliAddInputDelete(char c, CliState_t *state)
{
    uintBuf_t i;
    for (i = state->internalData.buffer.input.editPos; i<state->internalData.buffer.input.length; i++)
    {
        state->internalData.buffer.data[i] = state->internalData.buffer.data[i+1];
        fputc(state->internalData.buffer.data[i], state->myStdInOut);
    }
    if (state->internalData.buffer.input.length > state->internalData.buffer.input.editPos)
    {
        fputc(' ', state->myStdInOut);            
        i = state->internalData.buffer.input.length - state->internalData.buffer.input.editPos;
        state->internalData.buffer.input.length--;
        while (i>0)
        {
            fputc(ASCII_BS       , state->myStdInOut);
            i--;
        }                
    }
}

inline static void cliAddInputCR(char c, CliState_t *state)
{
    if (state->internalData.state == CLI_ST_HISTORY)
    {
        state->internalData.state = CLI_ST_READING_CMD;
    }
    fputc(ASCII_CR         , state->myStdInOut); /// user pressed [ENTER]
    fputc(ASCII_LF         , state->myStdInOut); /// echo CR and LF to terminal

    state->internalData.buffer.data[state->internalData.buffer.input.length++] = '\0';       /// add null termination to command
    state->internalData.buffer.input.editPos = 0;
}

void cmdlineInputFunc(char c, CliState_t *state)
{
/// VT100 handling
    if(state->internalData.vt100state == VT100_ST_CMD)  /// are we processing a VT100 command?
    {
        vt100procCmd(c, state);
        goto exit;
    }
    
    if ((state->internalData.vt100state == VT100_ST_ESC) && (c == '['))
    {
        state->internalData.vt100state = VT100_ST_CMD;
        goto exit;
    }

    state->internalData.vt100state = VT100_ST_DISABLED;  /// anything else, reset state
    
    if(c == ASCII_ESC)
    {
        state->internalData.vt100state = VT100_ST_ESC;
        goto exit;
    }
    
/// Regular handling
    if (state->internalData.buffer.input.length == CLI_STATE_BUF_LEN)
    {   /// Protection against buffer Overflow
        state->internalData.buffer.input.length--;
        fputc(ASCII_BEL, state->myStdInOut);
        fputc(ASCII_BS, state->myStdInOut);
    }
       
    if( (c >= 0x20) && (c < 0x7D) )
    {
        cliAddInputData(c, state);
        goto exit;
    }

    if(c == ASCII_CR)
    {
        cliAddInputCR(c, state);
        cliInputDataProcess(state);         /// command is complete, process it
        goto exit;
    }
    
    if(c == ASCII_BS)
    {
        cliAddInputBS(c, state);
        goto exit;
    }
    
    if(c == 0x7E)  //Delete
    {
        cliAddInputDelete(c, state);
        goto exit;
    }
exit:
    return;
}

void cliRepaint(CliState_t *state)
{
    uintBuf_t i;

/// carriage return
    fputc(ASCII_CR         , state->myStdInOut);
/// print fresh prompt
    cliPrintPrompt(state);
/// print the new command line buffer
    i = state->internalData.buffer.input.length;
    for (i=0; i < state->internalData.buffer.input.length; i++)
    {
        fputc(state->internalData.buffer.data[i], state->myStdInOut);
    }
    i = CLI_STATE_BUF_LEN - state->internalData.buffer.input.length;
    while (i--)
        fputc(' ', state->myStdInOut);

    i = CLI_STATE_BUF_LEN - state->internalData.buffer.input.editPos;
    while (i--)
        fputc(ASCII_BS,  state->myStdInOut);
}

void cliHistoryLoadAndMovePointer(CliState_t *state)
{
    state->internalData.buffer.input.editPos = 0;
    while (state->internalData.buffer.data[state->internalData.buffer.history.rdIdx] != '\0')
    {
        if (state->internalData.buffer.input.editPos >= state->internalData.buffer.history.wrIdx)
        {
            cliHistoryRemoveOldest(state);
        }
        
        if (state->internalData.buffer.input.editPos >= state->internalData.buffer.history.rdIdx)
        {
            state->internalData.buffer.data[state->internalData.buffer.input.editPos] = '\0';
            state->internalData.buffer.history.rdIdx = 0;
            break;
        }
        state->internalData.buffer.data[state->internalData.buffer.input.editPos] = state->internalData.buffer.data[state->internalData.buffer.history.rdIdx];
        state->internalData.buffer.history.rdIdx--;
        state->internalData.buffer.input.editPos++;
    }
    
    while (state->internalData.buffer.data[state->internalData.buffer.history.rdIdx] == '\0')
        state->internalData.buffer.history.rdIdx--;

    state->internalData.buffer.input.length = state->internalData.buffer.input.editPos;
    state->internalData.buffer.data[state->internalData.buffer.input.length] = '\0';
        
    state->internalData.buffer.history.depthIdx++;
    if (state->internalData.buffer.history.depthIdx >= state->internalData.buffer.history.depthLength)
    {
        state->internalData.buffer.history.depthIdx = 0;
        state->internalData.buffer.history.rdIdx = CLI_STATE_BUF_LEN - 1;
    }
}

void cliHistoryNavigateTop(CliState_t *state)
{
    state->internalData.buffer.history.rdIdx = CLI_STATE_BUF_LEN -1;
    state->internalData.buffer.history.depthIdx = 0;
}

static void cliHistoryNavigateYounger(CliState_t *state)
{
    if (state->internalData.buffer.history.depthIdx == 0)
        return;

    state->internalData.buffer.history.depthIdx--;
    if (state->internalData.buffer.history.depthIdx == 0)
    {
        state->internalData.buffer.history.rdIdx = CLI_STATE_BUF_LEN - 1;
        return;
    }
    
    if (state->internalData.buffer.history.rdIdx > CLI_STATE_BUF_LEN - 3)
    {
        CMD_msg("cliHistoryNavigateYounger Error Wrong RD idx\r\n");
        return;
    }
    
    state->internalData.buffer.history.rdIdx+=2;
    
    while (state->internalData.buffer.data[state->internalData.buffer.history.rdIdx] != '\0')
    {
        state->internalData.buffer.history.rdIdx++;
        if (state->internalData.buffer.history.rdIdx >= CLI_STATE_BUF_LEN)
        {
            CMD_msg("cliHistoryNavigateYounger Error 1\r\n");
            return;
        }
    }
    state->internalData.buffer.history.rdIdx--;
}

void cliInputDataProcess(CliState_t *state)
{
    cliInputDataParse(state);    
    
    uint8_t firstArgLen=0;
    const Command_t *tmpPtr = state->internalData.cmdList;                // Set list of commands. The list depends of the cli mode

/// find the first whitespace character in CmdlineBuffer
    while(state->internalData.buffer.data[firstArgLen] != '\0')
        firstArgLen++;

    if(firstArgLen > 0)                                                                // command was null or empty
    {
        do                                                                    // search command list for match with entered command
        {
#if USE_XC8
            if( !strncmp(state->internalData.buffer.data, tmpPtr->commandStr, firstArgLen) )      // user-entered command matched a command in the list                
            {
                memcpy(&state->internalData.cmd, tmpPtr, sizeof(Command_t));
#else
                memcpy_P(&state->internalData.cmd, tmpPtr, sizeof(Command_t));                       // read from flash. We need to copy it before
                if (state->internalData.cmd.commandStr == NULL)
                    break;
                if( !strncmp(state->internalData.buffer.data, tmp.commandStr, firstArgLen) )         // user-entered command matched a command in the list
                {                                                                 //
#endif
                if (state->internalData.cmd.maxArgC == 0)
                    state->internalData.cmd.maxArgC = CLI_STATE_MAX_ARGC;

                break;
            }
#if USE_XC8
            tmpPtr++;
#else
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

    if (state->internalData.cmd.commandFun == NULL)   
    {
        if (firstArgLen > 0)
        {
            cliPrintCommandNotFound(state);
            state->internalData.state = CLI_ST_WRONG_CMD;
        }
        else
        {
            state->internalData.state = CLI_ST_EMPTY;
        }
        cliPrintPrompt(state);                                          // output a new prompt
    }
    state->internalData.buffer.input.length = 0;
    state->internalData.buffer.input.editPos = 0;
}


static void cliInputDataParse(CliState_t *state)
{
    char *data = state->internalData.buffer.data;
    state->argv[0] = data;
    if (*data == '\0')
    {
        state->argc = 0;
    }
    else
    {
        state->argc = 1;
    }
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

static void cliHistorySave(CliState_t *state)
{
    
    uintBuf_t cmdDtaLen;
    uintBuf_t dstIdx;
    uintBuf_t srcIdx;    
    uint8_t i;
    
    cliUnparse(state);

    
    uintBuf_t isInHistoryUnderIdx = 0;
    
    dstIdx = CLI_STATE_BUF_LEN-1;
    
    for (i=0; i < state->internalData.buffer.history.depthLength; i++)
    {        
        srcIdx = 0;
        isInHistoryUnderIdx = dstIdx;

        
        while (state->internalData.buffer.data[dstIdx] != '\0')
        {
            if (isInHistoryUnderIdx != 0)
            {
                if (state->internalData.buffer.data[srcIdx] != state->internalData.buffer.data[dstIdx])
                    isInHistoryUnderIdx = 0;
                srcIdx++;            
            }
            dstIdx--;
        }
        if ((isInHistoryUnderIdx != 0) && (state->internalData.buffer.data[srcIdx] == '\0'))
            break;

        isInHistoryUnderIdx = 0;
        dstIdx--;
    }
        
    if (isInHistoryUnderIdx == 0)
    {
/// DROP
        while (2 * state->internalData.buffer.input.length + 2 > state->internalData.buffer.history.wrIdx)
        {
            cliHistoryRemoveOldest(state);
            if (state->internalData.buffer.history.depthLength == 0)
                break;
        }

/// SHIFT
    
        cmdDtaLen = state->internalData.buffer.input.length;
        dstIdx = state->internalData.buffer.history.wrIdx - cmdDtaLen;
        srcIdx = state->internalData.buffer.history.wrIdx + 1;
    }
    else //Change history order
    {
        state->internalData.buffer.history.depthLength--;                       //Move operation will remove one item from history
        dstIdx = isInHistoryUnderIdx - state->internalData.buffer.input.length;
        srcIdx = isInHistoryUnderIdx + 1; 
    }
    cmdDtaLen = CLI_STATE_BUF_LEN - srcIdx;
    memcpy(state->internalData.buffer.data + dstIdx, state->internalData.buffer.data + srcIdx, cmdDtaLen);
//Add given command on the end in reverse order

    
    dstIdx = CLI_STATE_BUF_LEN - 1;
    srcIdx = 0;
    while (state->internalData.buffer.data[srcIdx] != '\0')
    {
        state->internalData.buffer.data[dstIdx] = state->internalData.buffer.data[srcIdx];
        dstIdx--;
        srcIdx++;
    }
    state->internalData.buffer.data[dstIdx] = '\0';
    
    state->internalData.buffer.input.length = 0;
    state->internalData.buffer.input.editPos = 0;
    
    state->internalData.buffer.history.depthLength++;
    
    
    state->internalData.buffer.history.wrIdx = CLI_STATE_BUF_LEN - 1;

    for (i=0; i < state->internalData.buffer.history.depthLength; i++)
    {        
        while (state->internalData.buffer.data[state->internalData.buffer.history.wrIdx] != '\0')
            state->internalData.buffer.history.wrIdx--;
        
        state->internalData.buffer.history.wrIdx--;                
    }
}

static void cliUnparse(CliState_t *state)
{
    uint8_t i;
    const char *src;
    state->internalData.buffer.input.length = 0;
    for (i=0; i<state->argc; i++)
    {
        src = state->argv[i];
        while(*src != '\0')
        {   
            state->internalData.buffer.data[state->internalData.buffer.input.length] = *src;
            src++;
            state->internalData.buffer.input.length++;                
        }
        state->internalData.buffer.data[state->internalData.buffer.input.length] = ' ';
        state->internalData.buffer.input.length++;        
    }
    state->internalData.buffer.input.length--;
    state->internalData.buffer.data[state->internalData.buffer.input.length] = '\0';
    state->internalData.buffer.input.editPos = state->internalData.buffer.input.length;
}

void cliPrintCommandNotFound(CliState_t *state)
{
#if USE_XC8
    const char * ptr;
#else
    char * ptr;
#endif
    CMD_msg_const(cmdlineNotice);
/// print the offending command
    ptr = state->internalData.buffer.data;  //TODO Adam convert '\0' into ' '
    while((*ptr) && (*ptr != ' '))
        fputc(*ptr++    , state->myStdInOut);

    CMD_msg(": ");
    CMD_msg_const(cmdlineCmdNotFound);
    CMD_msg("\r\n");
}

void cmdPrintHistory(CliState_t *state)
{
    uintBuf_t i;
    uintBuf_t rdIdxOld = CLI_STATE_BUF_LEN -1;
    
    CMD_printf("History length %d (%d bytes)\r\n", state->internalData.buffer.history.depthLength, CLI_STATE_BUF_LEN - 1 - state->internalData.buffer.history.wrIdx);
    
    for (i=0; i < state->internalData.buffer.history.depthLength; i++)
    {
        CMD_printf("%2d (idx %3d) ", i+1, rdIdxOld);
        while (state->internalData.buffer.data[rdIdxOld] != '\0')
        {
            fputc(state->internalData.buffer.data[rdIdxOld--], state->myStdInOut);
        }
        rdIdxOld--;
        CMD_msg("\r\n");
    }
}

static void cliHistoryRemoveOldest(CliState_t *state)
{
    if (state->internalData.buffer.history.depthLength == 0)
    {
        CMD_msg("cliHistoryRemoveOldest Error: history is empty\r\n");
        return;
    }
    state->internalData.buffer.history.wrIdx++;

    while (state->internalData.buffer.data[++state->internalData.buffer.history.wrIdx] != '\0')    // Check if the previous command need to be overwritten
    {
        if (state->internalData.buffer.history.wrIdx >= CLI_STATE_BUF_LEN)
        {
            CMD_msg("cliHistoryRemoveOldest Error\r\n");
            break;
        }
    }
    state->internalData.buffer.history.wrIdx-=1;
    state->internalData.buffer.history.depthLength--;
}

uint8_t hexStrToDataN(uint8_t *data, const char *hexStr, uint8_t maxLen)
{
    uint8_t result = 0;
    const char *srcPntr = hexStr;
    uint8_t dataA;
    uint8_t dataB;
    while (result < maxLen)
    {
        if ((*srcPntr == '\0') || (*(srcPntr+1) == '\0'))
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

void cmdPrintHelp(CliState_t *state)
{
#if USE_XC8
    const Command_t *tmpPtr = state->internalData.cmdList;
#else
    Command_t  tmp;
    const Command_t *tmpPtr2 = state->internalData.cmdList;
    memcpy_P(&tmp, tmpPtr2, sizeof(Command_t));
    const Command_t *tmpPtr = &tmp;
#endif
    do
    {
        CMD_msg_const(tmpPtr->commandStr);
        CMD_msg("\t");
        CMD_msg_const(tmpPtr->commandHelpStr);
        CMD_msg("\r\n");
#if USE_XC8
        tmpPtr++;
#else
        tmpPtr2++;
        memcpy_P(&tmp, tmpPtr2, sizeof(Command_t));
#endif
    }
    while (tmpPtr->commandFun != NULL);
}

void cliPrintPrompt(CliState_t *state)
{
  // print a new command prompt
  switch (state->internalData.cliMode)
  {
    case NR_NORMAL:
      CMD_msg_const(cmdlinePromptNormal);
      break;
    case NR_ENABLE:
      CMD_msg_const(cmdlinePromptEnable);
      break;
    case NR_CONFIGURE:
      CMD_msg_const(cmdlinePromptConfigure);
      break;
    default:
      CMD_msg_const(cmdlinePromptNormal);
      break;
  }
}

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

#include "cmdline.h"        // Configuration
#include "vt100.h"          // vty100 constans


// Constans Strings
const char cmdlinePromptNormal[]    PROGMEM  = "DomOs>";
const char cmdlinePromptEnable[]    PROGMEM  = "DomOs#";
const char cmdlinePromptConfigure[] PROGMEM  = "DomOs@";
const char cmdlineNotice[]          PROGMEM  = "cmdline: ";
const char cmdlineCmdNotFound[]     PROGMEM  = "# nk";


// internal commands
static void cliRepaint(CliState_t *state);
static void cliHistoryShow(CliState_t *state);


# if CLI_STATE_INP_CMD_LEN > 255
static uint16_t cliHistorySave(CliState_t* state);
#else
static uint8_t cliHistorySave(CliState_t* state);
#endif


static void cliHistoryNavigateTop(CliState_t *state);
void cliHistoryNavigateYounger(CliState_t *state);
static void cliHistoryNavigateOlder(CliState_t *state);
static void cliHistoryLoad(CliState_t *state);

static void cliInputDataParse(CliState_t *state);
static void cliInputDataProcess(CliState_t *state);

static void cliPrintPrompt(CliState_t *state);
static void cliPrintCommandNotFound(CliState_t *state);

static void cliHistoryRemoveOldest(CliState_t *state);

//static void cliHistoryLoad(CliState_t *state);

/**
 * argc and argv to array.
 * @param state
 */
static void cliUnparse(CliState_t *state);

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





void cmdlineInputFunc(char c, CliState_t *state)
{
    uint8_t i;
    // process the received character

/// VT100 handling
/// are we processing a VT100 command?
    if(state->internalData.vty100.state == 2)
    {
        // we have already received ESC and [. Process the vt100 codeCmdlineExcBuffer
        switch(c)
        {
        case VT100_ARROWUP:
            //fprintf(state->myStdInOut, "\tCLI Arr Up, state = %d history (%d/%d)\r\n", state->internalData.state, state->internalData.history.depthIdx, state->internalData.history.depthLength);
            //cliPrintPrompt(state);
            switch (state->internalData.state)
            {
            case CLI_ST_EMPTY:
            case CLI_ST_READING_CMD:
                if (state->internalData.buffer.history.depthLength > 0)
                {
                    cliHistoryNavigateTop(state);
                    cliHistoryShow(state);
                    state->internalData.state = CLI_ST_HISTORY;
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
                if (state->internalData.buffer.history.depthIdx + 1 < state->internalData.buffer.history.depthLength)
                {
                    cliHistoryNavigateOlder(state);
                    cliHistoryShow(state);
                }
                else
                {
                    state->internalData.state = CLI_ST_READING_CMD;
                    cliRepaint(state);
                }
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
                if (state->internalData.buffer.history.depthIdx == 0)
                {
                    state->internalData.state = CLI_ST_READING_CMD;
                    cliRepaint(state);
                }
                else
                {
                    cliHistoryNavigateYounger(state);
                    cliHistoryShow(state);
                }
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
                fputc(ASCII_BEL        , state->myStdInOut);
            }
            break;

        case VT100_ARROWLEFT:
            switch (state->internalData.state)
            {
            case CLI_ST_EMPTY:
            case CLI_ST_READING_CMD:
                break;
                
            case CLI_ST_HISTORY:
                cliHistoryLoad(state);
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
                    fputc(ASCII_BS         , state->myStdInOut);
                }
                else
                {
                    fputc(ASCII_BEL        , state->myStdInOut);
                }
            }
            break;

        default:
            break;
        }
/// done, reset state
        state->internalData.vty100.state = 0;
        return;
    }
    else if(state->internalData.vty100.state == 1)
    {
/// we last received [ESC]
        if(c == '[')
        {
            state->internalData.vty100.state = 2;
            return;
        }
        else
            state->internalData.vty100.state = 0;
    }
    else
    {
/// anything else, reset state
        state->internalData.vty100.state = 0;
    }

/// Regular handling
/// Protection against buffer Overflow

    if (state->internalData.buffer.input.length >= state->internalData.buffer.history.wrIdx)
    {
        cliHistoryRemoveOldest(state);
    }

    if (state->internalData.buffer.input.length == CLI_STATE_INP_CMD_LEN)
    {
        state->internalData.buffer.input.length--;
        for (i=1; i < state->internalData.buffer.input.length; i++)
        {
            state->internalData.buffer.data[i-1] = state->internalData.buffer.data[i];
        }
    }
    
    if( (c >= 0x20) && (c < 0x7D) )
    {
        switch (state->internalData.state)
        {
        case CLI_ST_HISTORY:
            cliHistoryLoad(state);
            state->internalData.state = CLI_ST_READING_CMD;
            break;

        case CLI_ST_WRONG_CMD:
            state->internalData.state = CLI_ST_EMPTY;
            state->internalData.buffer.input.length = 0;
            state->internalData.buffer.input.editPos = 0;
            memset(state->internalData.buffer.data, 0, CLI_STATE_INP_CMD_LEN);
            break;
        
        case CLI_ST_EMPTY:
        case CLI_ST_READING_CMD:
            break;
        }
        
// character is printable
// is this a simple append
        if(state->internalData.buffer.input.editPos == state->internalData.buffer.input.length)
        {
/// echo character to the output
            fputc(c                , state->myStdInOut);
/// add it to the command line buffer
            state->internalData.buffer.data[state->internalData.buffer.input.editPos++] = c;
/// update buffer length
            state->internalData.buffer.input.length++;
        }
        else
        {
/// edit/cursor position != end of buffer
/// we're inserting characters at a mid-line edit position
/// make room at the insert point
            state->internalData.buffer.input.length++;
            for(i = state->internalData.buffer.input.length; i > state->internalData.buffer.input.editPos; i--)
                state->internalData.buffer.data[i] = state->internalData.buffer.data[i-1];
/// insert character
            state->internalData.buffer.data[state->internalData.buffer.input.editPos++] = c;
/// repaint
            cliRepaint(state);
/// reposition cursor
            for(i=state->internalData.buffer.input.editPos+1; i < state->internalData.buffer.input.length; i++)
                fputc(ASCII_BS         , state->myStdInOut);
        }
        state->internalData.state = CLI_ST_READING_CMD;
    }
/// handle special characters
    else if(c == ASCII_CR)
    {
        if (state->internalData.state == CLI_ST_HISTORY)
        {
            cliHistoryLoad(state);
            state->internalData.state = CLI_ST_READING_CMD;
        }
/// user pressed [ENTER]
/// echo CR and LF to terminal
        fputc(ASCII_CR         , state->myStdInOut);
        fputc(ASCII_LF         , state->myStdInOut);
/// add null termination to command
        state->internalData.buffer.data[state->internalData.buffer.input.length++] = '\0';
        state->internalData.buffer.input.editPos++;
/// command is complete, process it
        cliInputDataProcess(state);
    }
    else if(c == ASCII_BS)
    {
        if(state->internalData.buffer.input.editPos)
        {
/// is this a simple delete (off the end of the line)
            if(state->internalData.buffer.input.editPos == state->internalData.buffer.input.length)
            {
/// destructive backspace
/// echo backspace-space-backspace
                fputc(ASCII_BS         , state->myStdInOut);
                fputc(' '              , state->myStdInOut);
                fputc(ASCII_BS         , state->myStdInOut);
/// decrement our buffer length and edit position
                state->internalData.buffer.input.length--;
                state->internalData.buffer.input.editPos--;
            }
            else
            {
/// edit/cursor position != end of buffer
/// we're deleting characters at a mid-line edit position
/// shift characters down, effectively deleting
                state->internalData.buffer.input.length--;
                state->internalData.buffer.input.editPos--;
                for(i = state->internalData.buffer.input.editPos; i < state->internalData.buffer.input.length; i++)
                    state->internalData.buffer.data[i] = state->internalData.buffer.data[i+1];
/// repaint
                cliRepaint(state);
/// add space to clear leftover characters
//                fputc(' '              , state->myStdInOut);
/// reposition cursor
//                for(i = state->internalData.inputBuffer.editPos; i < (state->internalData.inputBuffer.length+1); i++)
//                    fputc(ASCII_BS       , state->myStdInOut);
            }
        }
        else
        {
/// else, ring the bell
            fputc(ASCII_BEL          , state->myStdInOut);
        }
    }
    else if(c == 0x7E)//ASCII_DEL)
    {
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
        //putc('X'       , state->myStdInOut);                
        //TODO Adam Verify it
    }
    else if(c == ASCII_ESC)
    {
        state->internalData.vty100.state = 1;
    }
}

void cliRepaint(CliState_t *state)
{
#if CLI_STATE_INP_CMD_LEN < 256
    uint8_t i;
#else
    uint16_t i;
#endif

/// carriage return
    fputc(ASCII_CR         , state->myStdInOut);
/// print fresh prompt
    cliPrintPrompt(state);
/// print the new command line buffer
    i = state->internalData.buffer.input.length;
    for (i=0; i < state->internalData.buffer.input.length; i++)
        fputc(state->internalData.buffer.data[i], state->myStdInOut);

    i = CLI_STATE_INP_CMD_LEN - state->internalData.buffer.input.length;
    while (i--)
        fputc(' ', state->myStdInOut);

    i = CLI_STATE_INP_CMD_LEN - state->internalData.buffer.input.editPos;
    while (i--)
        fputc(ASCII_BS,  state->myStdInOut);
}

void cliHistoryNavigateOlder(CliState_t *state)
{   
    state->internalData.buffer.history.depthIdx++;
    
    if (state->internalData.buffer.history.depthIdx >= state->internalData.buffer.history.depthLength)
    {
        fprintf(state->myStdInOut, "cliHistoryNavigateOlder Error 3");
        return;
    }

    while (state->internalData.buffer.data[state->internalData.buffer.history.rdIdx] != '\0')
    {
        state->internalData.buffer.history.rdIdx--;
        if (state->internalData.buffer.history.rdIdx <= state->internalData.buffer.history.wrIdx)
        {
            fprintf(state->myStdInOut, "cliHistoryNavigateOlder Error 1");
            break;
        }
    }

    state->internalData.buffer.history.rdIdx--;
    if (state->internalData.buffer.history.rdIdx <= state->internalData.buffer.history.wrIdx)
    {
        fprintf(state->myStdInOut, "cliHistoryNavigateOlder Error 2");
        return;
    }
}

void cliHistoryNavigateTop(CliState_t *state)
{
    state->internalData.buffer.history.rdIdx = CLI_STATE_INP_CMD_LEN -1;
    state->internalData.buffer.history.depthIdx = 0;
}

void cliHistoryNavigateYounger(CliState_t *state)
{
    if (state->internalData.buffer.history.depthIdx == 0)
        return;
    
    state->internalData.buffer.history.depthIdx--;
    
    state->internalData.buffer.history.rdIdx++;
    
    while (state->internalData.buffer.data[state->internalData.buffer.history.rdIdx] != '\0')
    {
        state->internalData.buffer.history.rdIdx++;
        if (state->internalData.buffer.history.rdIdx >= CLI_STATE_INP_CMD_LEN)
        {
            fprintf(state->myStdInOut, "cliHistoryNavigateYounger Error 1\r\n");
            return;
        }
    }
    state->internalData.buffer.history.rdIdx--;
}

void cliHistoryShow(CliState_t *state)
{
#if CLI_STATE_INP_CMD_LEN > 255
    uint16_t cmdLength = 0;    
    uint16_t tmp = 0;
#else
    uint8_t cmdLength = 0;
    uint8_t tmp = 0;
#endif
    
#if CLI_STATE_HISTORY_LEN > 255
    uint16_t srcIdx = state->internalData.history.rdIdx;
#else
    uint8_t srcIdx = state->internalData.buffer.history.rdIdx;    
#endif
    
    fputc('\r', state->myStdInOut);
    cliPrintPrompt(state);
    while (state->internalData.buffer.data[srcIdx] != '\0')
    {
        fputc(state->internalData.buffer.data[srcIdx], state->myStdInOut);
        srcIdx++;
        cmdLength++;
    }

    tmp = cmdLength;
    while (tmp < CLI_STATE_INP_CMD_LEN)
    {
        fputc(' ', state->myStdInOut);
        tmp++;
    }
    
    while (tmp > cmdLength)
    {
        fputc(ASCII_BS, state->myStdInOut);
        tmp--;
    }
}


void cliInputDataProcess(CliState_t *state)
{
    cliInputDataParse(state);    

    
    uint8_t firstArgLen=0;
    const Command_t *tmpPtr = state->internalData.cmdList;                // Set list of commands. The list depends of the cli mode

    while(state->internalData.buffer.data[firstArgLen] != '\0')                  // find first whitespace character in CmdlineBuffer
        firstArgLen++;                                                              // i determines the cammand length

    if(firstArgLen > 0)                                                                // command was null or empty
    {
        do                                                                    // search command list for match with entered command
        {
#if USE_XC8
            if( !strncmp(state->internalData.buffer.data, tmpPtr->commandStr, firstArgLen) )      // user-entered command matched a command in the list
            {
                memcpy(&state->internalData.cmd, tmpPtr, sizeof(Command_t));
#else
            memcpy_P(&state->internalData.cmd, tmpPtr, sizeof(Command_t));                  // read from flash. We need to copy it before
            if (state->internalData.cmd.commandStr == NULL)
                break;
            if( !strncmp(state->internalData.buffer.data, tmp.commandStr, firstArgLen) )         // user-entered command matched a command in the list
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

static void cliHistoryRemoveOldest(CliState_t *state)
{
    state->internalData.buffer.history.wrIdx+=2;

    while (state->internalData.buffer.data[state->internalData.buffer.history.wrIdx++] != '\0')    // Check if the previous command need to be overwritten
    {
        if (state->internalData.buffer.history.wrIdx >= CLI_STATE_INP_CMD_LEN)
        {
            fprintf(state->myStdInOut, "cliHistoryRemoveOldest Error\r\n");
            break;
        }
    }
    state->internalData.buffer.history.depthLength--;
}

static void cliHistoryDontSave(CliState_t *state)
{
    
}

# if CLI_STATE_INP_CMD_LEN > 255
static uint16_t cliHistorySave(CliState_t *state)
{
    uint16_t result;
#else
static uint8_t cliHistorySave(CliState_t *state)
{
    uint8_t result;
    
#endif
    
    result = 0;
#if 0
    uint8_t i;

    state->internalData.history.rdIdx = state->internalData.history.wrIdx;
    
    const char *src;
    
    for (i=0; i<state->argc; i++)
    {
        src = state->argv[i];
        while(*src != '\0')
        {
            cliHistoryRemoveOldest(state);
                
            state->internalData.history.data[state->internalData.history.wrIdx] = *src;
            cliHistoryIncWrPointer(state);
            
            src++;
            result++;
        }
        cliHistoryRemoveOldest(state);
                
        state->internalData.history.data[state->internalData.history.wrIdx] = (i+1 == state->argc) ? '\0' : ' ';
        cliHistoryIncWrPointer(state);

        result++;            
    }
    state->internalData.history.depthLength++;
#else
# if CLI_STATE_INP_CMD_LEN > 255
    uint16_t cmdDtaLen;
    uint16_t dstIdx;
    uint16_t srcIdx;
#else
    uint8_t cmdDtaLen;
    uint8_t dstIdx;
    uint8_t srcIdx;    
#endif
    
    cliUnparse(state);
    
    while (2 * state->internalData.buffer.input.length + 2 > state->internalData.buffer.history.wrIdx)
    {
        cliHistoryRemoveOldest(state);
    }
    
    cmdDtaLen = state->internalData.buffer.input.length;
    dstIdx = state->internalData.buffer.history.wrIdx - cmdDtaLen;
    srcIdx = state->internalData.buffer.history.wrIdx + 1;
    
//Move array implemented stack with leading '\0'
    memcpy(state->internalData.buffer.data + dstIdx, state->internalData.buffer.data + srcIdx, CLI_STATE_INP_CMD_LEN - srcIdx);
//Add given command on the end in reverse order

    state->internalData.buffer.history.wrIdx = dstIdx - 1;

    
    dstIdx = CLI_STATE_INP_CMD_LEN - 1;
    srcIdx = 0;
    while (state->internalData.buffer.data[srcIdx] != '\0')
    {
        state->internalData.buffer.data[dstIdx] = state->internalData.buffer.data[srcIdx];
        dstIdx--;
        srcIdx++;
        result++;
    }
    state->internalData.buffer.data[dstIdx] = '\0';
#endif
    return result;
}

static void cliHistoryLoad(CliState_t *state)
{
# if CLI_STATE_HISTORY_LEN > 255    
    uint16_t srcIdx;
#else
    uint8_t srcIdx;    
#endif
#if CLI_STATE_INP_CMD_LEN > 255
    uint16_t dstIdx;            
#else
    uint8_t dstIdx;        
#endif
    srcIdx = state->internalData.buffer.history.rdIdx;
    dstIdx = 0;

    memset(state->internalData.buffer.data, 0, CLI_STATE_INP_CMD_LEN);
    state->internalData.buffer.input.length = 0;
    state->internalData.buffer.input.editPos = 0;
    
       
    while (state->internalData.buffer.data[srcIdx] != '\0')
    {
        state->internalData.buffer.input.length++;
        state->internalData.buffer.input.editPos++;
        state->internalData.buffer.data[dstIdx] = state->internalData.buffer.data[srcIdx];
        srcIdx--;
        dstIdx++;
        if (dstIdx == CLI_STATE_INP_CMD_LEN)
        {
            fprintf(state->myStdInOut, "cliHistoryLoad Error\r\n");
            break;
        }
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
                
            if (state->internalData.buffer.input.length >= state->internalData.buffer.history.wrIdx)
                cliHistoryRemoveOldest(state);
        }
        state->internalData.buffer.data[state->internalData.buffer.input.length] 
                = ((i+1 == state->argc) || (state->internalData.buffer.input.length == CLI_STATE_INP_CMD_LEN)) ? '\0' : ' ';
        if (state->internalData.buffer.input.length >= state->internalData.buffer.history.wrIdx)
            cliHistoryRemoveOldest(state);
    }
    state->internalData.buffer.input.editPos = state->internalData.buffer.input.length;
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
    ptr = state->internalData.buffer.data;  //TODO Adam convert '\0' into ' '
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
#else
//    uint16_t i;
    uint16_t rdIdxOld;
#endif

    uint8_t i;
    
    rdIdxOld = CLI_STATE_INP_CMD_LEN -1;
    
    for (i=0; i < state->internalData.buffer.history.depthLength; i++)
    {
        fprintf(state->myStdInOut, "%2d\t", i+1);
        while (state->internalData.buffer.data[rdIdxOld] != '\0')
        {
            fputc(state->internalData.buffer.data[rdIdxOld--], state->myStdInOut);
        }
        rdIdxOld--;
        fprintf(state->myStdInOut, "\r\n");
    }
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

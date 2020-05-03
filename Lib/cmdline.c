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
static void cliRepaint(CliState_t *state);
static void cliHistoryShow(CliState_t *state);

static void cliHistoryNavigateTop(CliState_t *state);
static void cliHistoryNavigateYounger(CliState_t *state);
static void cliHistoryNavigateOlder(CliState_t *state);
static void cliHistoryLoad(CliState_t *state);

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



static inline void cliHistoryIncWrPointer(CliState_t *state);
static inline void cliHistoryIncRdPointerCharacter(CliState_t *state);
static inline void cliHistoryDecRdPointerCharacter(CliState_t *state);
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

#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
static void cliHistoryFreeIfNoSpace(CliState_t *state);
static void cliHistoryIncWrPointer(CliState_t *state);
static void cliHistoryIncRdPointerCharacter(CliState_t *state);
static void cliHistoryDecRdPointerCharacter(CliState_t *state);
static void cliHistoryDecRdPointerItem(CliState_t *state);
#else
static void cliHistoryRemoveOldest(CliState_t *state);
#endif

void cmdStateConfigure(CliState_t * state, FILE *stream, const Command_t *commands, enum CliModeState mode)
{
  memset(state, 0, sizeof(CliState_t));

  state->internalData.cliMode = mode;
  state->internalData.cmdList = commands;
 
  state->myStdInOut = stream;
  
#if CLI_SHARE_CMD_AND_HIST_BUF >= 0
  state->internalData.buffer.history.rdIdx = CLI_STATE_INP_CMD_LEN - 1;
  state->internalData.buffer.history.wrIdx = CLI_STATE_INP_CMD_LEN - 1;  
#endif
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
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
    state->internalData.buffer.input.data[state->internalData.buffer.input.editPos++] = c;  /// add it to the command line buffer     
#else
    state->internalData.buffer.data[state->internalData.buffer.input.editPos++] = c;        /// add it to the command line buffer
#endif
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
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
        state->internalData.buffer.input.data[i] = state->internalData.buffer.input.data[i-1]; /// Shift the data
#else
        state->internalData.buffer.data[i] = state->internalData.buffer.data[i-1];          /// Shift the data
#endif
    }
/// insert character
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
    state->internalData.buffer.input.data[state->internalData.buffer.input.editPos++] = c;  /// Insert the new character
#else
    state->internalData.buffer.data[state->internalData.buffer.input.editPos++] = c;        /// Insert the new character
#endif

}

inline static void _cliInputBufferClear(CliState_t *state)
{
#if CLI_SHARE_CMD_AND_HIST_BUF > 0
    memset(state->internalData.buffer.data, 0, state->internalData.buffer.input.length);
#else
    memset(state->internalData.buffer.input.data, 0, state->internalData.buffer.input.length);            
#endif
    state->internalData.buffer.input.length = 0;
    state->internalData.buffer.input.editPos = 0;    
}

inline static void cliAddInputData(char c, CliState_t *state)
{
    switch (state->internalData.state)
    {
    case CLI_ST_HISTORY:
        cliHistoryLoad(state);
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
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
                state->internalData.buffer.input.data[i] = state->internalData.buffer.input.data[i+1];   
#else
                state->internalData.buffer.data[i] = state->internalData.buffer.data[i+1];
#endif
            }
        }
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
        state->internalData.buffer.input.data[state->internalData.buffer.input.length] = '\0';   
#else
        state->internalData.buffer.data[state->internalData.buffer.input.length] = '\0';
#endif
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
#if CLI_STATE_INP_CMD_LEN > 255
    uint16_t i;
#else
    uint8_t i;
#endif
    for (i = state->internalData.buffer.input.editPos; i<state->internalData.buffer.input.length; i++)
    {
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
        state->internalData.buffer.input.data[i] = state->internalData.buffer.input.data[i+1];
        fputc(state->internalData.buffer.input.data[i], state->myStdInOut);
#else
        state->internalData.buffer.data[i] = state->internalData.buffer.data[i+1];
        fputc(state->internalData.buffer.data[i], state->myStdInOut);
#endif
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
        cliHistoryLoad(state);
        state->internalData.state = CLI_ST_READING_CMD;
    }
    fputc(ASCII_CR         , state->myStdInOut); /// user pressed [ENTER]
    fputc(ASCII_LF         , state->myStdInOut); /// echo CR and LF to terminal

#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
    state->internalData.buffer.input.data[state->internalData.buffer.input.length++] = '\0'; /// add null termination to command
#else
    state->internalData.buffer.data[state->internalData.buffer.input.length++] = '\0';       /// add null termination to command
#endif
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
    if (state->internalData.buffer.input.length == CLI_STATE_INP_CMD_LEN)
    {   /// Protection against buffer Overflow
        state->internalData.buffer.input.length--;
        fputc(ASCII_BEL, state->myStdInOut);
        fputc(ASCII_BS, state->myStdInOut);
    }
#if CLI_SHARE_CMD_AND_HIST_BUF > 0
    if (state->internalData.buffer.input.length >= state->internalData.buffer.history.wrIdx)
        cliHistoryRemoveOldest(state); /// Protection against input and history buffers overlaping
#endif
       
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
    {
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
        fputc(state->internalData.buffer.input.data[i], state->myStdInOut);
#else
        fputc(state->internalData.buffer.data[i], state->myStdInOut);
#endif
    }
    i = CLI_STATE_INP_CMD_LEN - state->internalData.buffer.input.length;
    while (i--)
        fputc(' ', state->myStdInOut);

    i = CLI_STATE_INP_CMD_LEN - state->internalData.buffer.input.editPos;
    while (i--)
        fputc(ASCII_BS,  state->myStdInOut);
}

void cliHistoryNavigateOlder(CliState_t *state)
{   
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0

#if CLI_STATE_HISTORY_LEN <= 256
    uint8_t cntr = 0;
#else
    uint16_t cntr = 0;
#endif
    
    state->internalData.buffer.history.depthIdx++;
    if (state->internalData.buffer.history.depthIdx >= state->internalData.buffer.history.depthLength)
        state->internalData.buffer.history.depthIdx = 0;

    
    while (state->internalData.buffer.history.data[state->internalData.buffer.history.rdIdx] != '\0')
    {
        cliHistoryDecRdPointerCharacter(state);
        cntr++;
        if (cntr > CLI_STATE_HISTORY_LEN)
        {
            fprintf(state->myStdInOut, "cliHistoryNavigateOlder ERROR 1 rdIdx = %d\r\n", state->internalData.buffer.history.rdIdx);
            return;
        }
    }

    while (state->internalData.buffer.history.data[state->internalData.buffer.history.rdIdx] == '\0')
    {
        cliHistoryDecRdPointerCharacter(state);
        cntr++;
        if (cntr > CLI_STATE_HISTORY_LEN)
        {
            fprintf(state->myStdInOut, "cliHistoryNavigateOlder ERROR 2\r\n");
            return;
        }
    }
    
    while (state->internalData.buffer.history.data[state->internalData.buffer.history.rdIdx] != '\0')
    {
        cliHistoryDecRdPointerCharacter(state);
        cntr++;
        if (cntr > CLI_STATE_HISTORY_LEN)
        {
            fprintf(state->myStdInOut, "cliHistoryNavigateOlder ERROR 3\r\n");
            return;
        }
    }
    cliHistoryIncRdPointerCharacter(state);
#endif
#if CLI_SHARE_CMD_AND_HIST_BUF > 0
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
#endif
}

void cliHistoryNavigateTop(CliState_t *state)
{
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
    if (state->internalData.buffer.history.depthLength > 0)
    {
        state->internalData.buffer.history.rdIdx = state->internalData.buffer.history.wrIdx;
        cliHistoryDecRdPointerItem(state);
    }    
    state->internalData.buffer.history.depthIdx = 0;
#else
    state->internalData.buffer.history.rdIdx = CLI_STATE_INP_CMD_LEN -1;
    state->internalData.buffer.history.depthIdx = 0;
#endif
}

static void cliHistoryNavigateYounger(CliState_t *state)
{
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
    #if CLI_STATE_HISTORY_LEN <= 256
    uint8_t cntr = 0;
#else
    uint16_t cntr = 0;
#endif
    
    while (state->internalData.buffer.history.data[state->internalData.buffer.history.rdIdx] != '\0')
    {
        cliHistoryIncWrPointer(state);
        cntr++;
        if (cntr > CLI_STATE_HISTORY_LEN)
            return;
    }

    while (state->internalData.buffer.history.data[state->internalData.buffer.history.rdIdx] == '\0')
    {
        cliHistoryIncWrPointer(state);
        cntr++;
        if (cntr > CLI_STATE_HISTORY_LEN)
            return;
    }

    if (state->internalData.buffer.history.depthIdx == 0)
        state->internalData.buffer.history.depthIdx = state->internalData.buffer.history.depthLength-1;
    else
        state->internalData.buffer.history.depthIdx--;

#else
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
#endif
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
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
    while (state->internalData.buffer.history.data[srcIdx] != '\0')
    {
        fputc(state->internalData.buffer.history.data[srcIdx], state->myStdInOut);
        srcIdx++;
        cmdLength++;
    }
#else
    while (state->internalData.buffer.data[srcIdx] != '\0')
    {
        fputc(state->internalData.buffer.data[srcIdx], state->myStdInOut);
        srcIdx--;
        cmdLength++;
    }    
#endif
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

/// find the first whitespace character in CmdlineBuffer
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
    while(state->internalData.buffer.input.data[firstArgLen] != '\0')
        firstArgLen++;
#else
    while(state->internalData.buffer.data[firstArgLen] != '\0')
        firstArgLen++;
#endif

    if(firstArgLen > 0)                                                                // command was null or empty
    {
        do                                                                    // search command list for match with entered command
        {
#if USE_XC8
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
            if( !strncmp(state->internalData.buffer.input.data, tmpPtr->commandStr, firstArgLen) )      // user-entered command matched a command in the list
#else
            if( !strncmp(state->internalData.buffer.data, tmpPtr->commandStr, firstArgLen) )      // user-entered command matched a command in the list                
#endif
            {
                memcpy(&state->internalData.cmd, tmpPtr, sizeof(Command_t));
#else
            memcpy_P(&state->internalData.cmd, tmpPtr, sizeof(Command_t));                       // read from flash. We need to copy it before
            if (state->internalData.cmd.commandStr == NULL)
                break;
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
            if( !strncmp(state->internalData.buffer.input.data, tmp.commandStr, firstArgLen) )   // user-entered command matched a command in the list
#else
            if( !strncmp(state->internalData.buffer.data, tmp.commandStr, firstArgLen) )         // user-entered command matched a command in the list
#endif
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
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
    char *data = state->internalData.buffer.input.data;
#else
    char *data = state->internalData.buffer.data;
#endif
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
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0    
    uint8_t i;
    result = 0;

    state->internalData.buffer.history.rdIdx = state->internalData.buffer.history.wrIdx;
    
    const char *src;
    
    for (i=0; i<state->argc; i++)
    {
        src = state->argv[i];
        while(*src != '\0')
        {
            cliHistoryFreeIfNoSpace(state);
            state->internalData.buffer.history.data[state->internalData.buffer.history.wrIdx] = *src;
            cliHistoryIncWrPointer(state);
            
            src++;
            result++;
        }
        cliHistoryFreeIfNoSpace(state);
               
        state->internalData.buffer.history.data[state->internalData.buffer.history.wrIdx] = (i+1 == state->argc) ? '\0' : ' ';
        cliHistoryIncWrPointer(state);

        result++;            
    }
#endif
    
#if CLI_SHARE_CMD_AND_HIST_BUF > 0    
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
    
    state->internalData.buffer.input.length = 0;
    state->internalData.buffer.input.editPos = 0;
    
#endif
    state->internalData.buffer.history.depthLength++;
    return result;
}

static void cliHistoryLoad(CliState_t *state)
{
#if CLI_STATE_INP_CMD_LEN > 255
    uint16_t dstIdx;            
#else
    uint8_t dstIdx;        
#endif
    dstIdx = 0;
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
# if CLI_STATE_HISTORY_LEN > 255    
    uint16_t srcIdx;
#else
    uint8_t srcIdx;    
#endif    
    srcIdx = state->internalData.buffer.history.rdIdx;


    memset(state->internalData.buffer.input.data, 0, CLI_STATE_INP_CMD_LEN);
    state->internalData.buffer.input.length = 0;
    state->internalData.buffer.input.editPos = 0;
    
       
    while (state->internalData.buffer.history.data[srcIdx] != '\0')
    {
        state->internalData.buffer.input.length++;
        state->internalData.buffer.input.editPos++;
        state->internalData.buffer.input.data[dstIdx] = state->internalData.buffer.history.data[srcIdx];
        srcIdx++;
        if (srcIdx == CLI_STATE_HISTORY_LEN)
            srcIdx = 0;
        dstIdx++;
        if (dstIdx == CLI_STATE_INP_CMD_LEN)
            break;
    } 
#endif
#if CLI_SHARE_CMD_AND_HIST_BUF > 0
# if CLI_STATE_INP_CMD_LEN > 255    
    uint16_t srcIdx;
#else
    uint8_t srcIdx;    
#endif    
    uint8_t tmp;
    
    srcIdx = state->internalData.buffer.history.rdIdx;

    state->internalData.buffer.input.length = 0;
    state->internalData.buffer.input.editPos = 0;
    
    while (state->internalData.buffer.data[srcIdx] != '\0')
    {
        state->internalData.buffer.input.length++;
        srcIdx--;
    }
    
    srcIdx = state->internalData.buffer.history.rdIdx;
    while (state->internalData.buffer.input.length >= state->internalData.buffer.history.wrIdx) //Can't load the history, because cmp buffer and history buffer overlap
    {   
        if (state->internalData.buffer.history.depthIdx + 1 < state->internalData.buffer.history.depthLength)
            cliHistoryRemoveOldest(state); //Just drop the history item
        else 
        {   //we want to load this item, so we can not drop it

            //Strings in history are reversed. Reverse id and shift on the beggining of the buffer            
            srcIdx =  state->internalData.buffer.history.wrIdx + 2;
            dstIdx =  srcIdx + state->internalData.buffer.input.length-1;

            while (srcIdx < dstIdx)
            {
                tmp = state->internalData.buffer.data[srcIdx];
                state->internalData.buffer.data[srcIdx] = state->internalData.buffer.data[dstIdx];
                state->internalData.buffer.data[dstIdx] = tmp;
                srcIdx++;
                dstIdx--;
            }
            
            //Shift on the beginning
            srcIdx =  state->internalData.buffer.history.wrIdx + 2;
            
            for (dstIdx = 0; dstIdx < state->internalData.buffer.input.length; dstIdx++, srcIdx++)
               state->internalData.buffer.data[dstIdx] = state->internalData.buffer.data[srcIdx];            

            //Update history history (drop item, that was moved to input buffer)
            state->internalData.buffer.history.wrIdx+=  state->internalData.buffer.input.length;
            for (srcIdx = state->internalData.buffer.input.length; srcIdx <= state->internalData.buffer.history.wrIdx; srcIdx++)
            {
                state->internalData.buffer.data[srcIdx] = '\0';
            }            
            srcIdx = state->internalData.buffer.input.length;            
            cliHistoryNavigateTop(state);
        }
    }
        
    while (state->internalData.buffer.data[srcIdx] != '\0')
    {
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
    state->internalData.buffer.data[state->internalData.buffer.input.length] = '\0';
#endif
}

static void cliUnparse(CliState_t *state)
{
    uint8_t i;
    const char *src;
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
    char *dst = state->internalData.buffer.input.data;

    for (i=0; i<state->argc; i++)
    {
        src = state->argv[i];
        while(*src != '\0')
        {                
            *dst = *src;
            src++;
            state->internalData.buffer.input.length++;
                
            if (state->internalData.buffer.input.length == CLI_STATE_INP_CMD_LEN)
            {
                *dst = '\0';
                break;
            }
            dst++;
        }
        *dst = ((i+1 == state->argc) || (state->internalData.buffer.input.length == CLI_STATE_INP_CMD_LEN)) ? '\0' : ' ';
        if (state->internalData.buffer.input.length == CLI_STATE_INP_CMD_LEN)
            break;
        dst++;
    }    
#endif
#if CLI_SHARE_CMD_AND_HIST_BUF > 0
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
#endif
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
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
    ptr = state->internalData.buffer.input.data;  //TODO Adam convert '\0' into ' '    
#else
    ptr = state->internalData.buffer.data;  //TODO Adam convert '\0' into ' '
#endif
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
#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
#if CLI_STATE_HISTORY_LEN <= 256
    uint8_t rdIdxOld;
    uint8_t cmdOldIdx;
#else
    uint16_t rdIdxOld;
    uint16_t cmdOldIdx;
#endif

    rdIdxOld = state->internalData.buffer.history.rdIdx;

    int tmp = 0;
    do
    {
        cmdOldIdx = state->internalData.buffer.history.rdIdx;
//        fprintf(state->myStdInOut, "\t Idx = %d\n", state->internalData.history.rdIdx);
        fputc('\t', state->myStdInOut);
        while (state->internalData.buffer.history.data[state->internalData.buffer.history.rdIdx] != '\0')
        {
            fputc(state->internalData.buffer.history.data[state->internalData.buffer.history.rdIdx], state->myStdInOut);
            cliHistoryIncRdPointerCharacter(state);
            tmp++;
            if (tmp >= CLI_STATE_HISTORY_LEN)
                break;
        }
        fputc('\r', state->myStdInOut);
        fputc('\n', state->myStdInOut);
                 
        state->internalData.buffer.history.rdIdx = cmdOldIdx;
        cliHistoryDecRdPointerItem(state);
        
        tmp++;
            if (tmp >= CLI_STATE_HISTORY_LEN)
                break;
    }
    while (rdIdxOld != state->internalData.buffer.history.rdIdx);
    
    state->internalData.buffer.history.rdIdx = rdIdxOld;
#endif
#if CLI_SHARE_CMD_AND_HIST_BUF > 0
#if CLI_STATE_HISTORY_LEN <= 256
//    uint8_t i;
    uint8_t rdIdxOld;
#else
//    uint16_t i;
    uint16_t rdIdxOld;
#endif

    uint8_t i;
    
    rdIdxOld = CLI_STATE_INP_CMD_LEN -1;
    
    fprintf(state->myStdInOut, "History length (%d/%d) rdIdx %d wrIdx 0x%02x\r\n", state->internalData.buffer.history.depthLength, state->internalData.buffer.history.depthIdx, state->internalData.buffer.history.rdIdx, state->internalData.buffer.history.wrIdx);
    
    for (i=0; i < state->internalData.buffer.history.depthLength; i++)
    {
        fprintf(state->myStdInOut, "%2d (0X%02x) ", i+1, rdIdxOld);
        while (state->internalData.buffer.data[rdIdxOld] != '\0')
        {
            fputc(state->internalData.buffer.data[rdIdxOld--], state->myStdInOut);
        }
        rdIdxOld--;
        fprintf(state->myStdInOut, "\r\n");
    }
#endif
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

#if CLI_SHARE_CMD_AND_HIST_BUF > 0
static void cliHistoryRemoveOldest(CliState_t *state)
{
    if (state->internalData.buffer.history.depthLength == 0)
    {
        fprintf(state->myStdInOut, "cliHistoryRemoveOldest Error: history is empty\r\n");
        return;
    }
    
    state->internalData.buffer.history.wrIdx+=2;

    while (state->internalData.buffer.data[++state->internalData.buffer.history.wrIdx] != '\0')    // Check if the previous command need to be overwritten
    {
        if (state->internalData.buffer.history.wrIdx >= CLI_STATE_INP_CMD_LEN)
        {
            fprintf(state->myStdInOut, "cliHistoryRemoveOldest Error\r\n");
            break;
        }
    }
    state->internalData.buffer.history.wrIdx-=1;

    state->internalData.buffer.history.depthLength--;
}
#endif

#if CLI_SHARE_CMD_AND_HIST_BUF <= 0
static void cliHistoryDecRdPointerItem(CliState_t *state)
{
#if CLI_STATE_HISTORY_LEN <= 256
    uint8_t noOfMoves;
#else
    uint16_t noOfMoves;    
#endif
    noOfMoves = 0;
    
    state->internalData.buffer.history.rdIdx = (state->internalData.buffer.history.rdIdx == 0) ? 
        state->internalData.buffer.history.rdIdx = CLI_STATE_HISTORY_LEN -1 : 
        state->internalData.buffer.history.rdIdx -1;
   
    while (state->internalData.buffer.history.data[state->internalData.buffer.history.rdIdx] == '\0')
    {
        noOfMoves++;
                
        state->internalData.buffer.history.rdIdx = (state->internalData.buffer.history.rdIdx == 0) ? 
            state->internalData.buffer.history.rdIdx = CLI_STATE_HISTORY_LEN -1 : 
            state->internalData.buffer.history.rdIdx -1;
    }

    while (state->internalData.buffer.history.data[state->internalData.buffer.history.rdIdx] != '\0')
    {
        noOfMoves++;
        state->internalData.buffer.history.rdIdx = (state->internalData.buffer.history.rdIdx == 0) ? 
            state->internalData.buffer.history.rdIdx = CLI_STATE_HISTORY_LEN -1 : 
            state->internalData.buffer.history.rdIdx -1;

        if (noOfMoves == CLI_STATE_HISTORY_LEN)
            return; // It should never happen
    }

    state->internalData.buffer.history.rdIdx++;
    if (state->internalData.buffer.history.rdIdx >= CLI_STATE_HISTORY_LEN)
        state->internalData.buffer.history.rdIdx = 0;
}

static void cliHistoryIncWrPointer(CliState_t *state)
{
    state->internalData.buffer.history.wrIdx++;
    if (state->internalData.buffer.history.wrIdx >= CLI_STATE_HISTORY_LEN)
        state->internalData.buffer.history.wrIdx = 0;    
}

static void cliHistoryIncRdPointerCharacter(CliState_t *state)
{
    if (++state->internalData.buffer.history.rdIdx >= CLI_STATE_HISTORY_LEN)
        state->internalData.buffer.history.rdIdx = 0;    
}

static void cliHistoryFreeIfNoSpace(CliState_t *state)
{
#if CLI_STATE_HISTORY_LEN > 255
    uint16_t tmpIdx = state->internalData.history.wrIdx;
#else
    uint8_t tmpIdx = state->internalData.buffer.history.wrIdx;    
#endif
    while (state->internalData.buffer.history.data[tmpIdx] != '\0')    // Check if the previous command need to be overwritten
    {
        state->internalData.buffer.history.data[tmpIdx] = '\0';
        tmpIdx++;
        if (tmpIdx == CLI_STATE_HISTORY_LEN)
            tmpIdx = 0;
    }
    if (tmpIdx != state->internalData.buffer.history.wrIdx)
        state->internalData.buffer.history.depthLength--;
}

static void cliHistoryDecRdPointerCharacter(CliState_t *state)
{
    if (state->internalData.buffer.history.rdIdx == 0)
        state->internalData.buffer.history.rdIdx = CLI_STATE_HISTORY_LEN-1;
    else
        state->internalData.buffer.history.rdIdx--;    
}

#endif


uint8_t hexStrToDataN(uint8_t *data, const char *hexStr, uint8_t maxLen)
{
    uint8_t result = 0;
    const char *srcPntr = hexStr;
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
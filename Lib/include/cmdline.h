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
#ifndef CMDLINE_H
#define CMDLINE_H

#include <stdio.h>
#include <avr/io.h>         // editor recognizes now types like uint8_t 


#include "softwareConfig.h"

//@{

  
// constants/macros/typdefs
struct CmdState;
struct Command;
enum   CliExecuteResult;

typedef struct Command         Command_t;
typedef struct CmdState        CmdState_t;
typedef enum CliExecuteResult  CliExRes_t;

typedef CliExRes_t (*CmdlineFuncPtrType)(CmdState_t *state);

enum CmdBufferHistory
{
  NOT_COPIED   = 0,
  COPIED       = 1
};

enum CliModeState
{
  NR_NORMAL,
  NR_ENABLE,
  NR_CONFIGURE,
  RESTRICTED_NORMAL
};

enum CliExecuteResult
{
  OK_SILENT =0,
  OK_INFORM,
  SYNTAX_ERROR,
  ERROR_SILENT,
  ERROR_INFORM,
  ERROR_OPERATION_NOT_ALLOWED
};

enum CliHistoryAction
{
  CMDLINE_HISTORY_SAVE,
  CMDLINE_HISTORY_PREV,
  CMDLINE_HISTORY_NEXT
};

struct CmdState
{
  char *cmdlineBuffer;                       ///< CLI buffer.
  char *cmdlineExcBuffer;                    ///< CLI processing buffer.
  char *cmdlineHistory[CMD_STATE_HISTORY];   ///< CLI history. History Size = 3. Sorry for Hardcodding

  uint8_t bufferMaxSize;                     ///< Total buffer size / CMD_STATE_HISTORY
  uint8_t cmdlineBufferLength;               ///< Number of writen chars in buffer
  uint8_t cmdlineBufferEditPos;              ///< Edit position in the buffer
 
  uint8_t historyWrIdx;                      ///< History write index (0 - CMD_STATE_HISTORY-1)   
  uint8_t historyDepthIdx;                   ///< History depth index. Read idx = (historyWrIdx - historyDepthIdx) & CMD_STATE_HISTORY_MASK
  enum CmdBufferHistory bufferHistoryState;  ///< Buffer history state
    
  uint8_t cmdlineInputVT100State;            ///< Commandline State TODO add enum type
  const char* command_str;                   ///< Executing command string
  const char* command_help_str;              ///< Executing command help string
  CmdlineFuncPtrType cmdlineExecFunction;    ///< Pointer to the funtion that match to the string writen in buffer
  uint8_t   argc;                            ///< Index of last argument
  
  FILE *myStdInOut;                          ///< Input / output stream descriptor
  
  uint8_t  errno;                            ///< Error number
  uint16_t err1;                             ///< Additional error info 1
  uint8_t  err2;                             ///< Additional error info 1
  
  enum CliModeState cliMode;                 ///< CLI mode (NORMAL, ENABLED, CONFIGURE)
  const Command_t *cmdList;                  ///< Each CLI mode has own command list
};

struct Command 
{
  const char           *commandStr;          ///< Command string
  const char           *commandHelpStr;      ///< Command help string
  CmdlineFuncPtrType   commandFun;           ///< Command function pointer
};


// functions


/**
 * call this function to pass input charaters from the user terminal
 * @param data   - pointer to the array with results
 * @param hexStr - pointer to the state array with XED string data
 * @param maxLen - length of data array
 * @return number of readed bytes
 */
uint8_t hexStrToDataN(uint8_t *data, const uint8_t *hexStr, uint8_t maxLen);


/**
 * call this function to pass input charaters from the user terminal
 * @param c     - new char
 * @param state - cli state
 */
void cmdlineInputFunc(char c, CmdState_t *state);

/**
 * call this function in task
 * @param state - cli state
 */
void cmdlineMainLoop(CmdState_t *state);

/**
 * Get last argument index.
 * @param state - cli state
 * @return last argument index
 */
uint8_t cmdLineGetLastArgIdx(CmdState_t *state);

// argument retrieval commands
/**
 * returns a string pointer to argument number [argnum] on the command line
 * @param argnum - argument no. Number of first arg is 1
 * @param state  - cli state
 * @return char pointer
 */
char* cmdlineGetArgStr(uint8_t argnum, CmdState_t *state);

/**
 * returns the decimal integer interpretation of argument number [argnum]
 * @param argnum - argument no. Number of first arg is 1
 * @param state  - cli state
 * @return long int
 */
long cmdlineGetArgInt (uint8_t argnum, CmdState_t *state);

/**
 * returns the hex integer interpretation of argument number [argnum]
 * @param argnum - argument no. Number of first arg is 1
 * @param state  - cli state
 */
long cmdlineGetArgHex (uint8_t argnum, CmdState_t *state);

/**
 * Print all commands available for cmdState and its description
 * @param state - command line interpreter state
 */
void cmdPrintHelp(CmdState_t *state);

/**
 * Konfiguruje strukturę do obsługi sesji interpretera poleceń
 * @param state            - wskaźnik do struktury ze stanem sesji interpretera poleceń
 * @param buffPtr          - wskaźnik do początku bufora. 1/4 bufora przeznaczona jest na buforowanie polecenia, 3/4 na zapamiętanie 3 ostatnich poleceń
 * @param bufferTotalSize  - długość przydzielonego bufora. Min 32 * CMD_STATE_HISTORY bajtów
 * @param *stream          - input/output stream
 * @param *commands        - pointer to the command table
 * @param mode             - command line interpreter mode
 */
void cmdStateConfigure(CmdState_t * state, char *buffPtr, uint16_t bufferTotalSize, FILE *stream, const Command_t *commands, enum CliModeState mode);

//@}
#endif

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
typedef struct CmdState        CliState_t;
typedef enum CliExecuteResult  CliExRes_t;

typedef CliExRes_t (*CmdlineFuncPtrType)(CliState_t *state);

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

struct Command 
{
  const char           *commandStr;          ///< Command string
  const char           *commandHelpStr;      ///< Command help string
  CmdlineFuncPtrType   commandFun;           ///< Command function pointer
  uint8_t              maxArgC;
};

//         +<--<--<-- Ascii char input <--<--<--<--<-- + 
//         |                                           |
//         +<- ALL OK <--<--+       +<--<-- KEY UP  <--+
//         |                |       |                  |
// EMPTY_INP_BUFFER -> READING_READING_CMD -->-->--> WRONG_CMD
//         |                  |  A
//         |                  V  |         
//         +-->-->-->--> HISTORY_Watching


typedef enum CLI_ST
{
    CLI_ST_EMPTY = 0,
    CLI_ST_READING_CMD = 1,
    CLI_ST_WRONG_CMD = 2,
    CLI_ST_HISTORY = 3
} CLI_ST_t;

struct CmdState
{
    uint8_t   argc;                            ///< Index of last argument
    const char *argv[CLI_STATE_MAX_ARGC];
    FILE *myStdInOut;                          ///< Input / output stream descriptor

    struct
    {
        uint8_t  errno;                        ///< Error number
        uint16_t err1;                         ///< Additional error info 1
        uint8_t  err2;                         ///< Additional error info 1
    } error;
    
    struct
    {
        CLI_ST_t state;        
      
        struct
        {
            char data [CLI_STATE_INP_CMD_LEN];
            
            struct
            {
#if CLI_STATE_INP_CMD_LEN < 256
                uint8_t length;               ///< Number of writen chars in buffer
                uint8_t editPos;              ///< Edit position in the buffer
#else                            
                uint16_t length;              ///< Number of writen chars in buffer
                uint16_t editPos;             ///< Edit position in the buffer
#endif
            } input;
            
            struct
            {
#if CLI_STATE_INP_CMD_LEN < 256
                uint8_t rdIdx;                ///< Read index
                uint8_t wrIdx;                ///< Write Index
#else
                uint16_t length;              ///< Read index
                uint16_t editPos;             ///< Write index
#endif
                uint8_t depthLength;          ///< Number of commands, that are stored
                uint8_t depthIdx;             ///< Current cmd idx. 0: last command in history buffer                
            } history;            
        } buffer;
        
        struct
        {
            uint8_t state;            ///< Commandline State TODO add enum type
        } vty100;

        Command_t cmd;
        
        enum CliModeState cliMode;                 ///< CLI mode (NORMAL, ENABLED, CONFIGURE)
        const Command_t *cmdList;                  ///< Each CLI mode has own command list
    } internalData;
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
void cmdlineInputFunc(char c, CliState_t *state);

/**
 * call this function in task
 * @param state - cli state
 */
void cliMainLoop(CliState_t *state);



void cmdPrintHistory(CliState_t *state);

/**
 * Print all commands available for cmdState and its description
 * @param state - command line interpreter state
 */
void cmdPrintHelp(CliState_t *state);

/**
 * Prepare the struct with LI state machine. Each instance of CLI has own state
 * @param state            - wskaźnik do struktury ze stanem sesji interpretera poleceń
 * @param *stream          - input/output stream
 * @param *commands        - pointer to the command table
 * @param mode             - command line interpreter mode
 */
void cmdStateConfigure(CliState_t * state, FILE *stream, const Command_t *commands, enum CliModeState mode);

//@}
#endif

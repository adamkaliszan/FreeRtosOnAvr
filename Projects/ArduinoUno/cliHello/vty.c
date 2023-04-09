#include "main.h"
#include "vty.h"
#include "hardwareConfig.h"
#include "configuration.h"
#include "softwareConfig.h"

#if LANG_EN
#include "vty_en.h"
#endif

#if LANG_PL
#include "vty_pl.h"
#endif

#ifndef LANG_VTY
#error "Vty Language not defined"
#endif


static CliExRes_t helpFunction               (CliState_t *state);
static CliExRes_t statusFunction             (CliState_t *state);
static CliExRes_t saveConfigFunction         (CliState_t *state);

static CliExRes_t disableFunction            (CliState_t *state);
static CliExRes_t enableFunction             (CliState_t *state);
static CliExRes_t configureModeFunction      (CliState_t *state);

const char okStr[]                        PROGMEM = "OK\r\n";
const char nlStr[]                        PROGMEM = "\r\n";
const char BladBuforaPozostaloBajtowStr[] PROGMEM = "!!! W budorze Rs485 pozostalo %d bajtow\r\n";

const  char * const errorStrings[] PROGMEM = {
  errorOK,
  errorNoFile,
  errorxModemFrameStartTimeout,
  errorxModemByteSendTimeout,
  errorxModemWrongFrameNo,
  errorxModemFrameFrameNoCorrectionNotMatch,
  errorxModemFrameCrc,
  errorxModemRemoteSideCan,
  errorxModemUnknownResponse,
  errorNoRemoteDevice,
  errorBootloaderNotResponding,
  errorOpenFile
};

const Command_t cmdListNormal[] PROGMEM =
{
  {cmd_help,      cmd_help_help,      helpFunction},
  {cmd_status,    cmd_help_status,    statusFunction},
  {cmd_enable,    cmd_help_enable,    enableFunction},
  {NULL, NULL, NULL}
};

const Command_t cmdListEnable[] PROGMEM =
{
  {cmd_help,      cmd_help_help,      helpFunction},
  {cmd_status,    cmd_help_status,    statusFunction},
  {cmd_disable,   cmd_help_disable,   disableFunction},
  {cmd_configure, cmd_help_configure, configureModeFunction},
  {NULL, NULL, NULL}
};

const Command_t cmdListConfigure[] PROGMEM =
{
  {cmd_help,         cmd_help_help,         helpFunction},
  {cmd_status,       cmd_help_status,       statusFunction},
  {cmd_conf_save,    cmd_help_conf_save,    saveConfigFunction},
  {cmd_enable,       cmd_help_enable,       enableFunction},
  {cmd_disable,      cmd_help_disable,      disableFunction},
  {NULL, NULL, NULL}
};

void VtyInit(CliState_t *state, FILE *stream)
{
  cmdStateConfigure(state, stream, &cmdListNormal[0], NR_NORMAL);
}

void printErrorInfo(CliState_t *state)
{
  if (state->error.errno != 0)
  {
    fprintf(state->myStdInOut, (const char *)(errorStrings + state->error.errno), state->error.err1, state->error.err2);
  }
  state->error.errno = 0;
  state->error.err1 = 0;
  state->error.err2 = 0;
}

static CliExRes_t enableFunction(CliState_t *state)
{
    CliExRes_t result = ERROR_OPERATION_NOT_ALLOWED;
    if (state->internalData.cliMode != RESTRICTED_NORMAL)
    {
        state->internalData.cmdList = cmdListEnable;
        state->internalData.cliMode = NR_ENABLE;
        result = OK_SILENT;
    }
    return result;
}
static CliExRes_t disableFunction(CliState_t *state)
{
    state->internalData.cmdList = cmdListNormal;
    if (state->internalData.cliMode != RESTRICTED_NORMAL)
    {
        state->internalData.cliMode = NR_NORMAL;
    }
    return OK_SILENT;
}

static CliExRes_t configureModeFunction(CliState_t *state)
{
    CliExRes_t result = ERROR_OPERATION_NOT_ALLOWED;
    if (state->internalData.cliMode == NR_ENABLE)
    {
       state->internalData.cmdList = cmdListConfigure;
        state->internalData.cliMode = NR_CONFIGURE;
        result = OK_SILENT;
    }
    return result;
}

// ************************** VTY API ***************************************************************************************
void printStatus(FILE *stream)
{
  fprintf_P(stream, PSTR(SYSTEM_NAME" ver "S_VERSION" build: "__DATE__", "__TIME__"\r\n"));
  //Print system state
  fprintf_P(stream, systemStateStr);
  fprintf_P(stream, statusNumberOfTasksStr,    uxTaskGetNumberOfTasks());
//  fprintf_P(stream, statusStaticHeapStateStr,  xPortGetFreeHeapSize(), configTOTAL_HEAP_SIZE);
//  printErrorInfo(state); //TODO fix and uncomment
}


// ************************** CLI Functions *********************************************************************************

static CliExRes_t helpFunction(CliState_t *state)
{
  cmdPrintHelp(state);
  return OK_SILENT;
}

static CliExRes_t statusFunction(CliState_t *state)
{
  printStatus(state->myStdInOut);
  return OK_SILENT;
}

static CliExRes_t saveConfigFunction(CliState_t *state)
{
  (void) state;
  saveConfiguration();
  return OK_SILENT;
}

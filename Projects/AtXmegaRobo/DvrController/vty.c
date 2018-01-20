#include "main.h"
#include "vty.h"
#include "hardwareConfig.h"
#include "configuration.h"
#include "softwareConfig.h"
#include "hardware.h"
#include "../../../LibRtos/include/sim900.h"

#if LANG_EN
#include "vty_en.h"
#endif

#if LANG_PL
#include "vty_pl.h"
#endif

#ifndef LANG_VTY
#error "Vty Language not defined"
#endif

extern volatile int16_t adcResults[];

static cliExRes_t helpFunction           (cmdState_t *state);
static cliExRes_t statusFunction         (cmdState_t *state);
static cliExRes_t enableFunction         (cmdState_t *state);
static cliExRes_t disableFunction        (cmdState_t *state);
static cliExRes_t configureModeFunction  (cmdState_t *state);
static cliExRes_t saveConfigFunction     (cmdState_t *state);

static cliExRes_t hc12modeFunction       (cmdState_t *state);
static cliExRes_t hc12channelFunction    (cmdState_t *state);
static cliExRes_t hc12baudrateFunction   (cmdState_t *state);
static cliExRes_t hc12powerFunction      (cmdState_t *state);
static cliExRes_t hc12statusFunction     (cmdState_t *state);

static cliExRes_t hc12sendForwardFunction    (cmdState_t *state);
static cliExRes_t hc12sendBackwordFunction   (cmdState_t *state);
static cliExRes_t hc12sendRotateLeftFunction (cmdState_t *state);
static cliExRes_t hc12sendRotateRightFunction(cmdState_t *state);
static cliExRes_t hc12sendStopFunction       (cmdState_t *state);

static cliExRes_t sim900OnFunction           (cmdState_t *state);
static cliExRes_t sim900OffFunction          (cmdState_t *state);
static cliExRes_t sim900atMode               (cmdState_t *state);

static cliExRes_t adcReadVoltagePwrSply      (cmdState_t *state);
static cliExRes_t adcReadCurrentPwrSply      (cmdState_t *state);


static cliExRes_t sendHC12(cmdState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t const cmdDta[]);
static cliExRes_t sendHC12AtCmd(cmdState_t *state, const char cmd[]);

static cliExRes_t sendHC12loopback(cmdState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t const cmdDta[]);

static cliExRes_t powerOnFunction           (cmdState_t *state);
static cliExRes_t powerOffFunction           (cmdState_t *state);

const char okStr[] PROGMEM = "OK\r\n";
const char nlStr[] PROGMEM = "\r\n";

const const char* const errorStrings[] PROGMEM = {
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

const command_t cmdListNormal[] PROGMEM =
{
  {cmd_help,             cmd_help_help,            helpFunction},
  {cmd_status,           cmd_help_status,          statusFunction},
  {cmd_enable,           cmd_help_enable,          enableFunction},
  {cmd_hc12forward,      cmd_help_hc12forward,     hc12sendForwardFunction},
  {cmd_hc12backward,     cmd_help_hc12backward,    hc12sendBackwordFunction},
  {cmd_hc12rotateLeft,   cmd_help_hc12rotateLeft,  hc12sendRotateLeftFunction},
  {cmd_hc12rotateRight,  cmd_help_hc12rotateRight, hc12sendRotateRightFunction},
  {cmd_hc12stop,         cmd_help_hc12stop,        hc12sendStopFunction},
  {cmd_powerOn,          cmd_help_powerOn,         powerOnFunction},
  {cmd_powerOff,         cmd_help_powerOff,        powerOffFunction},
  {cmd_rdVoltPwrSply,    cmd_help_rdVoltPwrSply,   adcReadVoltagePwrSply},
  {cmd_rdCurrPwrSply,    cmd_help_rdCurrPwrSply,   adcReadCurrentPwrSply},
  {NULL, NULL, NULL}
};

const command_t cmdListEnable[] PROGMEM =
{
  {cmd_help,             cmd_help_help,            helpFunction},
  {cmd_status,           cmd_help_status,          statusFunction},
  {cmd_disable,          cmd_help_disable,         disableFunction},
  {cmd_configure,        cmd_help_configure,       configureModeFunction},

  {cmd_HC12status,       cmd_help_HC12status,      hc12statusFunction},
  {cmd_hc12forward,      cmd_help_hc12forward,     hc12sendForwardFunction},
  {cmd_hc12backward,     cmd_help_hc12backward,    hc12sendBackwordFunction},
  {cmd_hc12rotateLeft,   cmd_help_hc12rotateLeft,  hc12sendRotateLeftFunction},
  {cmd_hc12rotateRight,  cmd_help_hc12rotateRight, hc12sendRotateRightFunction},
  {cmd_hc12stop,         cmd_help_hc12stop,        hc12sendStopFunction},

  {cmd_sim900on,         cmd_help_sim900on,        sim900OnFunction},
  {cmd_sim900off,        cmd_help_sim900off,       sim900OffFunction},
  {cmd_sim900at,         cmd_help_sim900at,        sim900atMode},
  {cmd_powerOn,          cmd_help_powerOn,         powerOnFunction},
  {cmd_powerOff,         cmd_help_powerOff,        powerOffFunction},

  {cmd_rdVoltPwrSply,    cmd_help_rdVoltPwrSply,   adcReadVoltagePwrSply},
  {cmd_rdCurrPwrSply,    cmd_help_rdCurrPwrSply,   adcReadCurrentPwrSply},
  {NULL, NULL, NULL}
};


const command_t cmdListConfigure[] PROGMEM =
{
  {cmd_help,         cmd_help_help,         helpFunction},
  {cmd_status,       cmd_help_status,       statusFunction},
  {cmd_HC12mode,     cmd_help_HC12mode,     hc12modeFunction},
  {cmd_HC12channel,  cmd_help_HC12channel,  hc12channelFunction},
  {cmd_HC12baudrate, cmd_help_HC12baudrate, hc12baudrateFunction},
  {cmd_HC12power,    cmd_help_HC12power,    hc12powerFunction},
  {cmd_HC12status,   cmd_help_HC12status,   hc12statusFunction},
  {cmd_conf_save,    cmd_help_conf_save,    saveConfigFunction},
  {cmd_enable,       cmd_help_enable,       enableFunction},
  {cmd_disable,      cmd_help_disable,      disableFunction},
  {NULL, NULL, NULL}
};

void VtyInit(cmdState_t* state, FILE *stream)
{
  cmdStateConfigure(state, (char *)(xmalloc(CLI_BUF_TOT_LEN)), CLI_BUF_TOT_LEN, stream, &cmdListNormal[0], NR_NORMAL);
}

void printErrorInfo(cmdState_t *state)
{
  if (state->errno != 0)
  {
    fprintf_P(state->myStdInOut, (const char*)(pgm_read_word(errorStrings + state->errno)), state->err1, state->err2);
  }
  state->errno = 0;
  state->err1 = 0;
  state->err2 = 0;
}

static cliExRes_t enableFunction(cmdState_t *state)
{
  if (state->cliMode != RESTRICTED_NORMAL)
  {
    state->cmdList = cmdListEnable;
    state->cliMode = NR_ENABLE;
    return OK_SILENT;
  }
  return ERROR_OPERATION_NOT_ALLOWED;
}
static cliExRes_t disableFunction(cmdState_t *state)
{
  state->cmdList = cmdListNormal;
  if (state->cliMode != RESTRICTED_NORMAL)
  {
    state->cliMode = NR_NORMAL;
  }
  return OK_SILENT;
}

// ************************** VTY API ***************************************************************************************
void printStatus(FILE *stream)
{
  fprintf_P(stream, PSTR(SYSTEM_NAME" ver "S_VERSION" build: "__DATE__", "__TIME__"\r\n"));

  fprintf_P(stream, PSTR("Hc12 config:\r\n"));
  fprintf_P(stream, PSTR("\tmode    %d\r\n"), confHC12mode);
  fprintf_P(stream, PSTR("\tbaud    %d\r\n"), confHC12baud);
  fprintf_P(stream, PSTR("\tchannel %d\r\n"), confHC12channel);
  fprintf_P(stream, PSTR("\tpower   %d\r\n"), confHC12power);


//  uint16_t res = ADCA.CH0RES;
/*  ADCA.CH0.CTRL    = ADC_CH_INPUTMODE_SINGLEENDED_gc;        //Pojedyncze wejście
  ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;                  //PB0

  ADCA.CH1.CTRL    = ADC_CH_INPUTMODE_SINGLEENDED_gc;        //Pojedyncze wejście
  ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;                  //PB0


  ADCA.CTRLA     = ADC_ENABLE_bm | ADC_CH0START_bm | ADC_CH1START_bm;          //Włączenie przetwornika AC oraz uruchomienie pomiaru na kanale 0

  while (ADCA.CH0.INTFLAGS==0);
  uint16_t res = ADCA.CH0RES;
  ADCA.CH0.INTFLAGS=1;

  while (ADCA.CH1.INTFLAGS==0);
  uint16_t res2 = ADCA.CH1RES;
  ADCA.CH1.INTFLAGS=1;


  ADCA.CTRLA = ADC_ENABLE_bm;
  fprintf_P(stream, PSTR("Pwr: %d + %d/128 V\r\n"), res>>7, res&0x7F);
  fprintf_P(stream, PSTR("     %d + %d/32 A\r\n"),  res2>>5, res2&0x1F);

*/

  //Print system state
}

static cliExRes_t adcReadVoltagePwrSply      (cmdState_t *state)
{
  int16_t val = adcResults[1];//  getVoltagePwrSply();

  float voltage = val/58.5365;
  fprintf_P(state->myStdInOut, PSTR("Voltage pwr sply %f\r\n"), voltage);

  val = adcResults[2];//getVoltageInternal();
  voltage = val/196.0;
  fprintf_P(state->myStdInOut, PSTR("Voltage internal %f (%d)\r\n"), voltage, val);

  val = adcResults[3];//getTemperatureInternal();
  //voltage = val/200;
  fprintf_P(state->myStdInOut, PSTR("Temperature raw %d\r\n"), val);


  return OK_SILENT;
}

static cliExRes_t adcReadCurrentPwrSply      (cmdState_t *state)
{
  int16_t val = getCurrentPwrSply();
  fprintf_P(state->myStdInOut, PSTR("Current %d\r\n"), val);
  return OK_SILENT;
}


// ************************** CLI Functions *********************************************************************************

static cliExRes_t statusFunction(cmdState_t *state)
{
  printStatus(state->myStdInOut);
  return OK_SILENT;
}

static cliExRes_t helpFunction(cmdState_t *state)
{
  cmdPrintHelp(state);
  return OK_SILENT;
}

static cliExRes_t saveConfigFunction(cmdState_t *state)
{
  (void) state;
  saveConfiguration();
  return OK_SILENT;
}

static cliExRes_t configureModeFunction(cmdState_t *state)
{
  if (state->cliMode == NR_ENABLE)
  {
    state->cmdList = cmdListConfigure;
    state->cliMode = NR_CONFIGURE;
    return OK_SILENT;
  }
  return ERROR_OPERATION_NOT_ALLOWED;
}

static cliExRes_t powerOnFunction           (cmdState_t *state)
{
  if (state->argc != 1)
    return ERROR_INFORM;

  uint8_t outNo = cmdlineGetArgInt(1, state);
  if (outNo > 12 || outNo == 0)
    return ERROR_INFORM;

  setOut(outNo);
  return OK_SILENT;
}

static cliExRes_t powerOffFunction           (cmdState_t *state)
{
  if (state->argc != 1)
    return ERROR_INFORM;

  uint8_t outNo = cmdlineGetArgInt(1, state);
  if (outNo > 12 || outNo == 0)
    return ERROR_INFORM;

  clearOut(outNo);
  return OK_SILENT;
}


static cliExRes_t hc12sendForwardFunction    (cmdState_t *state)
{
  tlvMsgMoveDta_t dta;
  dta.duration = 0;
  dta.pwmLeft  = 50;
  dta.pwmRight = 50;

  if (state->argc == 1)
    dta.pwmLeft = dta.pwmRight = cmdlineGetArgInt(1, state);
  if (state->argc >=2)
  {
    dta.pwmLeft = cmdlineGetArgInt(1, state);
    dta.pwmRight = cmdlineGetArgInt(2, state);
  }
  sendHC12(state, 0, FORWARD, sizeof(tlvMsgMoveDta_t), (uint8_t *) &dta);
  return OK_SILENT;
}

static cliExRes_t hc12sendBackwordFunction   (cmdState_t *state)
{
  tlvMsgMoveDta_t dta;
  dta.duration = 0;
  dta.pwmLeft  = 50;
  dta.pwmRight = 50;

  if (state->argc == 1)
    dta.pwmLeft = dta.pwmRight = cmdlineGetArgInt(1, state);
  if (state->argc >=2)
  {
    dta.pwmLeft = cmdlineGetArgInt(1, state);
    dta.pwmRight = cmdlineGetArgInt(2, state);
  }
  sendHC12(state, 0, BACKWORD, sizeof(tlvMsgMoveDta_t), (uint8_t *) &dta);
  return OK_SILENT;
}


static cliExRes_t hc12sendRotateLeftFunction    (cmdState_t *state)
{
  tlvMsgMoveDta_t dta;
  dta.duration = 0;
  dta.pwmLeft  = 50;
  dta.pwmRight = 50;

  if (state->argc == 1)
    dta.pwmLeft = dta.pwmRight = cmdlineGetArgInt(1, state);
  if (state->argc >=2)
  {
    dta.pwmLeft = cmdlineGetArgInt(1, state);
    dta.pwmRight = cmdlineGetArgInt(2, state);
  }
  sendHC12(state, 0, ROTATE_LEFT, sizeof(tlvMsgMoveDta_t), (uint8_t *) &dta);

  return OK_SILENT;
}

static cliExRes_t hc12sendRotateRightFunction    (cmdState_t *state)
{
  tlvMsgMoveDta_t dta;
  dta.duration = 0;
  dta.pwmLeft  = 50;
  dta.pwmRight = 50;

  if (state->argc == 1)
    dta.pwmLeft = dta.pwmRight = cmdlineGetArgInt(1, state);
  if (state->argc >=2)
  {
    dta.pwmLeft = cmdlineGetArgInt(1, state);
    dta.pwmRight = cmdlineGetArgInt(2, state);
  }
  sendHC12(state, 0, ROTATE_RIGHT, sizeof(tlvMsgMoveDta_t), (uint8_t *) &dta);

  return OK_SILENT;
}

static cliExRes_t hc12sendStopFunction       (cmdState_t *state)
{
  sendHC12(state, 0, STOP, 0, NULL);
  return OK_SILENT;
}

static cliExRes_t sendHC12AtCmd(cmdState_t *state, const char cmd[])
{
  if (xSemaphoreTake(Hc12semaphore, 10) == pdTRUE)
  {
    vTaskDelay(2);
    HC12setAtMode();
    vTaskDelay(25);
    fprintf_P(state->myStdInOut, cmd, cmdlineGetArgStr(1, state));
    fprintf_P(&hc12Stream,       cmd, cmdlineGetArgStr(1, state));

    uint8_t val;
    while (xQueueReceive(xHC12Rec, &val, 100) == pdTRUE)
    {
        fputc(val, state->myStdInOut);
    }

    HC12setTransparentMode();
    xSemaphoreGive(Hc12semaphore );
    return OK_SILENT;
  }
  else
  {
    return ERROR_INFORM;
  }
}

static cliExRes_t sendHC12(cmdState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t const cmdDta[])
{
  (void) state;
  tlvMsg_t msg;
  msg.sync = TLV_SYNC;
  msg.address = addr;
  msg.type = type;
  msg.dtaLen = len;

  tlvCalculateCrcSepDta(&msg, cmdDta);

  sendTlvMsgDta(&msg, cmdDta, &hc12Stream);

  return OK_INFORM;
}

static cliExRes_t sendHC12loopback(cmdState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t const cmdDta[])
{
  (void) state;
  tlvMsg_t msg;
  msg.sync = TLV_SYNC;
  msg.address = addr;
  msg.type = type;
  msg.dtaLen = len;

  tlvCalculateCrcSepDta(&msg, cmdDta);

  sendTlvMsgDta(&msg, cmdDta, &hc12FakeStream);

  return OK_INFORM;
}

static cliExRes_t hc12modeFunction       (cmdState_t *state)
{
    confHC12mode = cmdlineGetArgInt(1, state);
    return sendHC12AtCmd(state, PSTR("AT+FU%s\r\n"));
}

static cliExRes_t hc12channelFunction    (cmdState_t *state)
{
    confHC12channel = cmdlineGetArgInt(1, state);
    return sendHC12AtCmd(state, PSTR("AT+C%s\r\n"));
}

static cliExRes_t hc12baudrateFunction   (cmdState_t *state)
{
    confHC12baud = cmdlineGetArgInt(1, state);
    return sendHC12AtCmd(state, PSTR("AT+B%s\r\n"));
}

static cliExRes_t hc12powerFunction      (cmdState_t *state)
{
    confHC12power = cmdlineGetArgInt(1, state);
    return sendHC12AtCmd(state, PSTR("AT+P%s\r\n"));
}

static cliExRes_t hc12statusFunction     (cmdState_t *state)
{
    return sendHC12AtCmd(state, PSTR("AT+RX\r\n"));
}

static cliExRes_t sim900OnFunction           (cmdState_t *state)
{
    (void) state;
    sim900pwrOn();
    return OK_INFORM;
}

static cliExRes_t sim900OffFunction          (cmdState_t *state)
{
    (void) state;
    sim900pwrOffHw();
    return OK_INFORM;
}

static cliExRes_t sim900atMode               (cmdState_t *state)
{
    uint8_t znak;
    fprintf_P(state->myStdInOut, PSTR("Press ^z to exit at mode\r\n"));
    for ( ; ;)
    {
        if (xQueueReceive(xVtyRec, &znak, 0) == pdTRUE)
        {
            if (znak == 0x1A)
                break;
            xQueueSend(xSIM900Tx, &znak, 0);
        }
        if (xQueueReceive(xSIM900Rec, &znak, 0) == pdTRUE)
            fputc(znak, state->myStdInOut);
    }
    return OK_INFORM;
}

/*
static void printTable(FILE *stream)
{
  fprintf_P(stream, PSTR("\r\nAddress Table:\r\n\r\n"));  //Print system state

  uint16_t i;
  for(i=0; i<256; i++)
  {
    fprintf_P(stream, PSTR("%3d -> %3d\t"), i, translateTable[i]);
    i++ ;
    fprintf_P(stream, PSTR("%3d -> %3d\t"), i, translateTable[i]);
    i++ ;
    fprintf_P(stream, PSTR("%3d -> %3d\t"), i, translateTable[i]);
    i++ ;
    fprintf_P(stream, PSTR("%3d -> %3d\r\n"), i, translateTable[i]);
  }
  fprintf_P(stream, PSTR("\r\n"));
}
*/

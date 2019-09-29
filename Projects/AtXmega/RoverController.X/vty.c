#include <stdlib.h>

#include "main.h"
#include "vty.h"
#include "hardwareConfig.h"
#include "configuration.h"
#include "softwareConfig.h"
#include "hardware.h"
#include "sim900.h"

#include "cmdline.h"

#include "hc12.h"

#include "drvPAL/i2cPAL.h"

#if LANG_EN
#include "vty_en.h"
#endif

#if LANG_PL
#include "vty_pl.h"
#endif

#ifndef LANG_VTY
#error "Vty Language not defined"
#endif

static CliExRes_t helpFunction           (CliState_t *state);
static CliExRes_t historyFunction        (CliState_t *state);
static CliExRes_t statusFunction         (CliState_t *state);
static CliExRes_t enableFunction         (CliState_t *state);
static CliExRes_t disableFunction        (CliState_t *state);
static CliExRes_t configureModeFunction  (CliState_t *state);
static CliExRes_t saveConfigFunction     (CliState_t *state);

static CliExRes_t forwardFunction        (CliState_t *state);
static CliExRes_t backwordFunction       (CliState_t *state);
static CliExRes_t rotateLeftFunction     (CliState_t *state);
static CliExRes_t rotateRightFunction    (CliState_t *state);
static CliExRes_t stopFunction           (CliState_t *state);

static CliExRes_t pwrFunction            (CliState_t *state);

static CliExRes_t hc12modeFunction       (CliState_t *state);
static CliExRes_t hc12channelFunction    (CliState_t *state);
static CliExRes_t hc12baudrateFunction   (CliState_t *state);
static CliExRes_t hc12powerFunction      (CliState_t *state);
static CliExRes_t hc12statusFunction     (CliState_t *state);

static CliExRes_t pwmSetFreq             (CliState_t *state);

static CliExRes_t hc12sendForwardFunction    (CliState_t *state);
static CliExRes_t hc12sendBackwordFunction   (CliState_t *state);
static CliExRes_t hc12sendRotateLeftFunction (CliState_t *state);
static CliExRes_t hc12sendRotateRightFunction(CliState_t *state);
static CliExRes_t hc12sendStopFunction       (CliState_t *state);

static CliExRes_t sim900OnFunction           (CliState_t *state);
static CliExRes_t sim900OffFunction          (CliState_t *state);
static CliExRes_t sim900atMode               (CliState_t *state);

static CliExRes_t twiWtiteAndRead            (CliState_t *state);


static CliExRes_t sendHC12(CliState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t const cmdDta[]);
static CliExRes_t sendHC12AtCmd(CliState_t *state, const char cmd[]);

static CliExRes_t sendHC12loopback(CliState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t const cmdDta[]);




const char okStr[] PROGMEM = "OK\r\n";
const char nlStr[] PROGMEM = "\r\n";

const char* const errorStrings[] PROGMEM = {
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
  {cmd_help,            cmd_help_help,            helpFunction},
  {cmd_history,         cmd_help_history,         historyFunction},
  {cmd_status,          cmd_help_status,          statusFunction},
  {cmd_enable,          cmd_help_enable,          enableFunction},
  {cmd_forward,         cmd_help_forward,         forwardFunction},
  {cmd_backward,        cmd_help_backward,        backwordFunction},
  {cmd_rotateLeft,      cmd_help_rotateLeft,      rotateLeftFunction},
  {cmd_rotateRight,     cmd_help_rotateRight,     rotateRightFunction},
  {cmd_stop,            cmd_help_stop,            stopFunction},
  {cmd_hc12forward,     cmd_help_hc12forward,     hc12sendForwardFunction},
  {cmd_hc12backward,    cmd_help_hc12backward,    hc12sendBackwordFunction},
  {cmd_hc12rotateLeft,  cmd_help_hc12rotateLeft,  hc12sendRotateLeftFunction},
  {cmd_hc12rotateRight, cmd_help_hc12rotateRight, hc12sendRotateRightFunction},
  {cmd_hc12stop,        cmd_help_hc12stop,        hc12sendStopFunction},
  {cmd_twiWriteAndRead, cmd_help_twiWriteAndStop, twiWtiteAndRead},
  {NULL, NULL, NULL}
};

const Command_t cmdListEnable[] PROGMEM =
{
  {cmd_help,        cmd_help_help,        helpFunction},
  {cmd_history,     cmd_help_history,     historyFunction},
  {cmd_status,      cmd_help_status,      statusFunction},
  {cmd_disable,     cmd_help_disable,     disableFunction},
  {cmd_configure,   cmd_help_configure,   configureModeFunction},
  {cmd_forward,     cmd_help_forward,     forwardFunction},
  {cmd_backward,    cmd_help_backward,    backwordFunction},
  {cmd_rotateLeft,  cmd_help_rotateLeft,  rotateLeftFunction},
  {cmd_rotateRight, cmd_help_rotateRight, rotateRightFunction},
  {cmd_stop,        cmd_help_stop,        stopFunction},

  {cmd_pwr,         cmd_help_pwr,         pwrFunction},

  {cmd_HC12status,      cmd_help_HC12status,      hc12statusFunction},

  {cmd_hc12forward,     cmd_help_hc12forward,     hc12sendForwardFunction},
  {cmd_hc12backward,    cmd_help_hc12backward,    hc12sendBackwordFunction},
  {cmd_hc12rotateLeft,  cmd_help_hc12rotateLeft,  hc12sendRotateLeftFunction},
  {cmd_hc12rotateRight, cmd_help_hc12rotateRight, hc12sendRotateRightFunction},
  {cmd_hc12stop,        cmd_help_hc12stop,        hc12sendStopFunction},

  {cmd_sim900on,        cmd_help_sim900on,        sim900OnFunction},
  {cmd_sim900off,       cmd_help_sim900off,       sim900OffFunction},
  {cmd_sim900at,        cmd_help_sim900at,        sim900atMode},

  {NULL, NULL, NULL}
};


const Command_t cmdListConfigure[] PROGMEM =
{
  {cmd_help,         cmd_help_help,         helpFunction},
  {cmd_history,      cmd_help_history,      historyFunction},
  {cmd_status,       cmd_help_status,       statusFunction},
  {cmd_HC12mode,     cmd_help_HC12mode,     hc12modeFunction},
  {cmd_HC12channel,  cmd_help_HC12channel,  hc12channelFunction},
  {cmd_HC12baudrate, cmd_help_HC12baudrate, hc12baudrateFunction},
  {cmd_HC12power,    cmd_help_HC12power,    hc12powerFunction},
  {cmd_HC12status,   cmd_help_HC12status,   hc12statusFunction},
  {cmd_conf_save,    cmd_help_conf_save,    saveConfigFunction},
  {cmd_enable,       cmd_help_enable,       enableFunction},
  {cmd_disable,      cmd_help_disable,      disableFunction},
  {cmd_pwm_freq,     cmd_help_pwm_freq,     pwmSetFreq},
  {NULL, NULL, NULL}
};

void VtyInit(CliState_t* state, FILE *stream)
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

// ************************** VTY API ***************************************************************************************
void printStatus(FILE *stream)
{
#ifdef USE_XC8
  fprintf(stream, SYSTEM_NAME" ver "S_VERSION" build: "__DATE__", "__TIME__"\r\n");
  fprintf(stream, "PWR status: 4v3 %s, RPI 3v3 %s, RPI 4v3 %s\r\n", isPwr4v3() ? "On": "Off", isPwr3v3rpi() ? "On": "Off", isPwr4v3rpi() ? "On": "Off");

  fprintf(stream, "Hc12 config:\r\n");
  fprintf(stream, "\tmode    %d\r\n", confHC12mode);
  fprintf(stream, "\tbaud    %d\r\n", confHC12baud);
  fprintf(stream, "\tchannel %d\r\n", confHC12channel);
  fprintf(stream, "\tpower   %d\r\n", confHC12power);    
#else
  fprintf_P(stream, PSTR(SYSTEM_NAME" ver "S_VERSION" build: "__DATE__", "__TIME__"\r\n"));
  fprintf_P(stream, PSTR("PWR status: 4v3 %s, RPI 3v3 %s, RPI 4v3 %s\r\n"), isPwr4v3() ? "On": "Off", isPwr3v3rpi() ? "On": "Off", isPwr4v3rpi() ? "On": "Off");

  fprintf_P(stream, PSTR("Hc12 config:\r\n"));
  fprintf_P(stream, PSTR("\tmode    %d\r\n"), confHC12mode);
  fprintf_P(stream, PSTR("\tbaud    %d\r\n"), confHC12baud);
  fprintf_P(stream, PSTR("\tchannel %d\r\n"), confHC12channel);
  fprintf_P(stream, PSTR("\tpower   %d\r\n"), confHC12power);
#endif

//  uint16_t res = ADCA.CH0RES;
  ADCA.CH0.CTRL    = ADC_CH_INPUTMODE_SINGLEENDED_gc;        //Pojedyncze wejście
  ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN8_gc;                  //PB0

  ADCA.CH1.CTRL    = ADC_CH_INPUTMODE_SINGLEENDED_gc;        //Pojedyncze wejście
  ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN9_gc;                  //PB0


  ADCA.CTRLA     = ADC_ENABLE_bm | ADC_CH0START_bm | ADC_CH1START_bm;          //Włączenie przetwornika AC oraz uruchomienie pomiaru na kanale 0

  while (ADCA.CH0.INTFLAGS==0);
  uint16_t res = ADCA.CH0RES;
  ADCA.CH0.INTFLAGS=1;

  while (ADCA.CH1.INTFLAGS==0);
  uint16_t res2 = ADCA.CH1RES;
  ADCA.CH1.INTFLAGS=1;


  ADCA.CTRLA = ADC_ENABLE_bm;

#ifdef USE_XC8
  fprintf(stream, "Pwr: %d + %d/128 V\r\n", res>>7, res&0x7F);
  fprintf(stream, "     %d + %d/32 A\r\n",  res2>>5, res2&0x1F);
#else
  fprintf_P(stream, PSTR("Pwr: %d + %d/128 V\r\n"), res>>7, res&0x7F);
  fprintf_P(stream, PSTR("     %d + %d/32 A\r\n"),  res2>>5, res2&0x1F);  
#endif
  //Print system state
}


// ************************** CLI Functions *********************************************************************************

static CliExRes_t statusFunction(CliState_t *state)
{
    printStatus(state->myStdInOut);
    return OK_SILENT;
}

static CliExRes_t historyFunction(CliState_t *state)
{
    cmdPrintHistory(state);
    return OK_SILENT;
}

static CliExRes_t helpFunction(CliState_t *state)
{
    cmdPrintHelp(state);
    return OK_SILENT;
}

static CliExRes_t saveConfigFunction(CliState_t *state)
{
    (void) state;
    saveConfiguration();
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


static CliExRes_t forwardFunction        (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);

    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12loopback(state, 0, FORWARD, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);

    return OK_SILENT;
}

static CliExRes_t backwordFunction       (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12loopback(state, 0, BACKWORD, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);

    return OK_SILENT;
}
static CliExRes_t rotateLeftFunction     (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12loopback(state, 0, ROTATE_LEFT, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);

    return OK_SILENT;
}

static CliExRes_t rotateRightFunction    (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12loopback(state, 0, ROTATE_RIGHT, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);

    return OK_SILENT;
}

static CliExRes_t stopFunction           (CliState_t *state)
{
    uint8_t dta[1];
    dta[0] = 0;
    sendHC12loopback(state, 0, STOP, 1, dta);

    return OK_SILENT;
}


static CliExRes_t pwrFunction           (CliState_t *state)
{
    if (state->argc <2)
        return ERROR_INFORM;

    uint8_t devNo = atoi(state->argv[1]);
    uint8_t devState = atoi(state->argv[2]);

    if (devState == 0)
    {
        switch (devNo)
        {
            case 1: pwrOff4v3();     break;
            case 2: pwrOff3v3rpi();  break;
            case 3: pwrOff4v3rpi();  break;
            default:                 break;
        }
    }
    else
    {
        switch (devNo)
        {
            case 1: pwrOn4v3();      break;
            case 2: pwrOn3v3rpi();   break;
            case 3: pwrOn4v3rpi();   break;
            default:                 break;
        }
    }
    return OK_SILENT;
}

static CliExRes_t hc12sendForwardFunction    (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
          dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12(state, 0, FORWARD, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);
    return OK_SILENT;
}

static CliExRes_t hc12sendBackwordFunction   (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12(state, 0, BACKWORD, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);
    return OK_SILENT;
}


static CliExRes_t hc12sendRotateLeftFunction    (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
      dta.pwmLeft = atoi(state->argv[1]);
      dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12(state, 0, ROTATE_LEFT, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);

    return OK_SILENT;
}

static CliExRes_t hc12sendRotateRightFunction    (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12(state, 0, ROTATE_RIGHT, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);

    return OK_SILENT;
}

static CliExRes_t hc12sendStopFunction       (CliState_t *state)
{
  sendHC12(state, 0, STOP, 0, NULL);
  return OK_SILENT;
}

static CliExRes_t sendHC12AtCmd(CliState_t *state, const char cmd[])
{
    if (xSemaphoreTake(Hc12semaphore, 10) == pdTRUE)
    {
        vTaskDelay(2);
        HC12setAtMode();
        vTaskDelay(25);
#ifdef USE_XC8
        fprintf(state->myStdInOut, cmd, state->argv[1]);
        fprintf(&hc12Stream,       cmd, state->argv[1]);
#else
    fprintf_P(state->myStdInOut, cmd, cmdlineGetArgStr(1, state));
    fprintf_P(&hc12Stream,       cmd, cmdlineGetArgStr(1, state));    
#endif
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

static CliExRes_t sendHC12(CliState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t const cmdDta[])
{
  (void) state;
  TlvMsg_t msg;
  msg.sync = TLV_SYNC;
  msg.address = addr;
  msg.type = type;
  msg.dtaLen = len;

  tlvCalculateCrcSepDta(&msg, cmdDta);

  sendTlvMsgDta(&msg, cmdDta, &hc12Stream);

  return OK_INFORM;
}

static CliExRes_t sendHC12loopback(CliState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t const cmdDta[])
{
  (void) state;
  TlvMsg_t msg;
  msg.sync = TLV_SYNC;
  msg.address = addr;
  msg.type = type;
  msg.dtaLen = len;

  tlvCalculateCrcSepDta(&msg, cmdDta);

  sendTlvMsgDta(&msg, cmdDta, &hc12FakeStream);

  return OK_INFORM;
}


static CliExRes_t twiWtiteAndRead(CliState_t *state)
{
    uint8_t result;
    uint8_t tmpDta[8];
    
    
  uint8_t address = atoi(state->argv[1]);
  uint8_t rdDtaLen = atoi(state->argv[2]);
  uint8_t wrDtaLen = hexStrToDataN(tmpDta, (const uint8_t*)state->argv[3], 8);

  result = TWI_MasterWriteRead(&hardwarePAL.twiSensors, address, tmpDta, wrDtaLen, rdDtaLen);
  
  if (result)
  {
      fprintf(state->myStdInOut, "Result: ");
      vTaskDelay(100);
      for(result = 0; result < rdDtaLen; result++)
      {
          fprintf(state->myStdInOut, "0x%02x ", hardwarePAL.twiSensors.readData[result]);
      }
      fprintf(state->myStdInOut, "\n");
      
      return OK_INFORM;    
  }
  return ERROR_INFORM;
}


static CliExRes_t pwmSetFreq (CliState_t *state)
{
    uint8_t preskaler = atoi(state->argv[1]);
    if (preskaler > 7 || preskaler < 4)
        return ERROR_INFORM;

    TCC0.CTRLA &= 0xF0;
    TCC0.CTRLA |= preskaler;

    return OK_INFORM;
}

static CliExRes_t hc12modeFunction       (CliState_t *state)
{
    confHC12mode = atoi(state->argv[1]);
    return sendHC12AtCmd(state, PSTR("AT+FU%s\r\n"));
}

static CliExRes_t hc12channelFunction    (CliState_t *state)
{
    confHC12channel = atoi(state->argv[1]);
    return sendHC12AtCmd(state, PSTR("AT+C%s\r\n"));
}

static CliExRes_t hc12baudrateFunction   (CliState_t *state)
{
    confHC12baud = atoi(state->argv[1]);
    return sendHC12AtCmd(state, PSTR("AT+B%s\r\n"));
}

static CliExRes_t hc12powerFunction      (CliState_t *state)
{
    confHC12power = atoi(state->argv[1]);
    return sendHC12AtCmd(state, PSTR("AT+P%s\r\n"));
}

static CliExRes_t hc12statusFunction     (CliState_t *state)
{
    return sendHC12AtCmd(state, PSTR("AT+RX\r\n"));
}

static CliExRes_t sim900OnFunction           (CliState_t *state)
{
    (void) state;
    sim900pwrOn();
    return OK_INFORM;
}

static CliExRes_t sim900OffFunction          (CliState_t *state)
{
    (void) state;
    sim900pwrOffHw();
    return OK_INFORM;
}

static CliExRes_t sim900atMode               (CliState_t *state)
{
    uint8_t znak;
#ifdef USE_XC8
    fprintf(state->myStdInOut, "Press ^z to exit at mode\r\n");
#else
    fprintf_P(state->myStdInOut, PSTR("Press ^z to exit at mode\r\n"));
#endif
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

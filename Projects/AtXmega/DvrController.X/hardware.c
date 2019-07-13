#include "hardware.h"
#include <avr/pgmspace.h>
#include <stddef.h>

//xQueueHandle      xSpiRx;             /// Kolejka z odebranymi bajtami z SPI. Blokuje transmisję do czasu zakończenia wysyłania poprzedniego bajtu
//xQueueHandle      xSpiRxEnc;


//#define

uint8_t ReadCalibrationByte( uint8_t index )
{
    uint8_t result;

    /* Load the NVM Command register to read the calibration row. */
    NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
    result = pgm_read_byte(index);

    /* Clean up NVM Command register. */
    NVM_CMD = NVM_CMD_NO_OPERATION_gc;

    return( result );
}

void hardwareInit(void)
{
    /** PORT A
    0 - ACS711 - current sensor
    1 - Voltage divider - power supply voltage sensor
    2 - Out 5
    3 - Out 6
    4 - Out 7
    5 - Out 8
    6 - Out 9
    7 - Out 10          **/
    PORTA.DIR=0xFC;
    PORTA.OUT=0x00;

    /** PORT B
    0 - Out 12
    1 - Out 11
    2 - In 1
    3 - In 2           **/
    PORTB.DIR=0x03;
    PORTB.OUT=0x00;

    /** PORT C
    0 - I2C
    1 - I2C
    2 - UART VTY RxD
    3 - UART VTY TxD
    4 - In 3
    5 - In 4
    6 - UART radio RxD
    7 - UART radio TxD   **/
    PORTC.REMAP=((1<<PORT_TC0A_bp)|(1<<PORT_TC0B_bp)) ;
    PORTC.DIR=0x88;
    PORTC.OUT=0x00;

    /** PORT D
    0 - Radio set/reset
    1 - Sim900 reset
    2 - Sim900 RxD
    3 - Sim900 TxD
    4 - Sim900 PWR key
    5 - Rs485 En
    6 - Rs485 Rx
    7 - Rs485 Tx         **/
    PORTD.DIR=0xBB;
    PORTD.OUT=0x00;

    /** PORT E
    0 Out 3
    1 Out 4
    2 RPI RxD
    3 RPI TxD          **/
    PORTE.DIR=0x0B;
    PORTE.OUT=0x00;

    /** PORT R
    0 Out 1
    1 Out 2            **/
    PORTR.DIR=0x03;
    PORTR.OUT=0x00;

    //PORT

    /// A/C init

    ADCA.CTRLA     = ADC_ENABLE_bm;// | ADC_CH0START_bm | ADC_FLUSH_bm;       //Włączenie przetwornika AC oraz uruchomienie pomiaru na kanale 0
    ADCA.CTRLB     = ADC_CONMODE_bm | ADC_RESOLUTION_12BIT_gc; //ADC_IMPMODE_bm | ADC_CURRLIMIT0_bm | ADC_CURRLIMIT1_bm
                 //| ADC_FREERUN_bm | ADC_RESOLUTION0_bm | ADC_RESOLUTION1_bm |

    ADCA.REFCTRL   = ADC_REFSEL_INT1V_gc | ADC_BANDGAP_bm | ADC_TEMPREF_bm;          //BANDGAP enable, TempRef enable, Vref = VCC/1.6 V
    ADCA.EVCTRL    = 0;           /** Event Control      */
    ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc;    //prescaler 256, f=125kHz

    ADCA.CALL = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0) ); //0x20
    ADCA.CALH = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1) ); //0x21

    //Curent sensor ADS 711
    ADCA.CH0.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc; //(Channel<<3);
    ADCA.CH0.INTCTRL = 0;
    ADCA.CH0.INTFLAGS = 0;
    ADCA.CH0.RES = 0;
    ADCA.CH0.SCAN = 0;

    //Voltage divider
    ADCA.CH1.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc; //(Channel<<3);
    ADCA.CH1.INTCTRL = 0;
    ADCA.CH1.INTFLAGS = 0;
    ADCA.CH1.RES = 0;
    ADCA.CH1.SCAN = 0;

    //Internal voltage
    ADCA.CH2.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_INTERNAL_gc;
    ADCA.CH2.MUXCTRL = ADC_CH_MUXINT_SCALEDVCC_gc; //(Channel<<3);
    ADCA.CH2.INTCTRL = 0;
    ADCA.CH2.INTFLAGS = 0;
    ADCA.CH2.RES = 0;
    ADCA.CH2.SCAN = 0;

    //Internal temperature sensor
    ADCA.CH3.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_INTERNAL_gc;
    ADCA.CH3.MUXCTRL = ADC_CH_MUXINT_TEMP_gc; //(Channel<<3);
    ADCA.CH3.INTCTRL = 0;
    ADCA.CH3.INTFLAGS = 0;
    ADCA.CH3.RES = 0;
    ADCA.CH3.SCAN = 0;

//    uint8_t x =  ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, TEMPSENSE0) );
//    uint8_t y =  ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, TEMPSENSE1) );
}

void readAdcResults()
{
    adcResults[0] = ADCA.CH0RES;
    adcResults[1] = ADCA.CH1RES;
    adcResults[2] = ADCA.CH2RES;
    adcResults[3] = ADCA.CH3RES;

    ADCA.CTRLA |= ADC_ENABLE_bm | ADC_FLUSH_bm | ADC_CH0START_bm| ADC_CH1START_bm| ADC_CH2START_bm| ADC_CH3START_bm;
}


int16_t getCurrentPwrSply()
{
//    ADCA.CH0.INTCTRL = 0 ; // No interrupt

///TODO Don't copy yourself
    for(uint8_t Waste = 0; Waste<2; Waste++)
    {
        ADCA.CH0.CTRL |= ADC_CH_START_bm; // Start conversion
        while ((ADCA.INTFLAGS & ADC_CH0IF_bm)==0) ; // Wait for complete
        ADCA.INTFLAGS = ADC_CH0IF_bm ;
    }
    return ADCA.CH0RES ;
}

int16_t getVoltagePwrSply()
{
    ADCA.CH1.CTRL |= ADC_CH_START_bm; // Start conversion
    while ((ADCA.INTFLAGS & ADC_CH1IF_bm)==0)
        ; // Wait for complete
    ADCA.INTFLAGS = ADC_CH1IF_bm;
    return ADCA.CH1RES ;
}

int16_t getVoltageInternal()
{
    ADCA.CH2.CTRL |= ADC_CH_START_bm; // Start conversion
    while ((ADCA.INTFLAGS & ADC_CH2IF_bm)==0)
        ; // Wait for complete
    ADCA.INTFLAGS = ADC_CH2IF_bm;
    return ADCA.CH2RES ;
}

int16_t getTemperatureInternal()
{
    ADCA.CH3.CTRL |= ADC_CH_START_bm; // Start conversion
    while ((ADCA.INTFLAGS & ADC_CH3IF_bm)==0)
        ; // Wait for complete
    ADCA.INTFLAGS = ADC_CH3IF_bm;
    return ADCA.CH3RES ;
}


inline void setOut1   (void) { PORTE.OUTSET=0x01; } //E0
inline void clearOut1 (void) { PORTE.OUTCLR=0x01; }
inline void setOut2   (void) { PORTE.OUTSET=0x02; } //E1
inline void clearOut2 (void) { PORTE.OUTCLR=0x02; }

inline void setOut3   (void) { PORTR.OUTSET=0x01; } //R0
inline void clearOut3 (void) { PORTR.OUTCLR=0x01; }
inline void setOut4   (void) { PORTR.OUTSET=0x02; } //R1
inline void clearOut4 (void) { PORTR.OUTCLR=0x02; }

inline void setOut5   (void) { PORTA.OUTSET=0x04; } //A2
inline void clearOut5 (void) { PORTA.OUTCLR=0x04; }
inline void setOut6   (void) { PORTA.OUTSET=0x08; } //A3
inline void clearOut6 (void) { PORTA.OUTCLR=0x08; }

inline void setOut7   (void) { PORTA.OUTSET=0x10; } //A4
inline void clearOut7 (void) { PORTA.OUTCLR=0x10; }
inline void setOut8   (void) { PORTA.OUTSET=0x20; } //A5
inline void clearOut8 (void) { PORTA.OUTCLR=0x20; }

inline void setOut9   (void) { PORTA.OUTSET=0x40; } //A6
inline void clearOut9 (void) { PORTA.OUTCLR=0x40; }
inline void setOut10  (void) { PORTA.OUTSET=0x80; } //A7
inline void clearOut10(void) { PORTA.OUTCLR=0x80; }

inline void setOut11  (void) { PORTB.OUTSET=0x02; } //B1
inline void clearOut11(void) { PORTB.OUTCLR=0x02; }
inline void setOut12  (void) { PORTB.OUTSET=0x01; } //B0
inline void clearOut12(void) { PORTB.OUTCLR=0x01; }


void setOut  (uint8_t no)
{
    switch (no)
    {
    case 1:
        setOut1();
        break;
    case 2:
        setOut2();
        break;
    case 3:
        setOut3();
        break;
    case 4:
        setOut4();
        break;
    case 5:
        setOut5();
        break;
    case 6:
        setOut6();
        break;
    case 7:
        setOut7();
        break;
    case 8:
        setOut8();
        break;
    case 9:
        setOut9();
        break;
    case 10:
        setOut10();
        break;
    case 11:
        setOut11();
        break;
    case 12:
        setOut12();
        break;
    }
}

void clearOut(uint8_t no)
{
    switch (no)
    {
    case 1:
        clearOut1();
        break;
    case 2:
        clearOut2();
        break;
    case 3:
        clearOut3();
        break;
    case 4:
        clearOut4();
        break;
    case 5:
        clearOut5();
        break;
    case 6:
        clearOut6();
        break;
    case 7:
        clearOut7();
        break;
    case 8:
        clearOut8();
        break;
    case 9:
        clearOut9();
        break;
    case 10:
        clearOut10();
        break;
    case 11:
        clearOut11();
        break;
    case 12:
        clearOut12();
        break;
    }
}

void HC12setAtMode()
{
  PORTD.OUTCLR = PIN0_bm;
}

void HC12setTransparentMode()
{
  PORTD.OUTSET = PIN0_bm;
}

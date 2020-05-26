#include "hardware.h"
#include <avr/pgmspace.h>
#include <stddef.h>

#include "FreeRTOS.h"
#include "task.h"

#include "../../drv/include/twi.h"

#define BAUDRATE	100000
#define TWI_BAUDSETTING TWI_BAUD(F_SYS, BAUDRATE)

HardwarePAL_t hardwarePAL;


//xQueueHandle      xSpiRx;             /// Kolejka z odebranymi bajtami z SPI. Blokuje transmisję do czasu zakończenia wysyłania poprzedniego bajtu
//xQueueHandle      xSpiRxEnc;

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
    /// PORT A MOSTKI H
    //0 - (18)        4 - B1 in
    //1 - (17)        5 - A1 in
    //2 - (20)        6 - B2 in
    //3 - (19)        7 - A2 in
    PORTA.DIR=0xFF;
    PORTA.OUT=0x00;

    ///PORT B - Złącze 20 pin
    //0 - 13          2 - 14
    //1 - 14          3 - 15
    PORTB.DIR=0x00;
    PORTB.OUT=0x00;

    ///PORT C
    // 0 I2C              // 4 PWMA
    // 1 I2C              // 5 PWMB
    // 2 UART VTY RxD     // 6 UART radio RxD
    // 3 UART VTY TxD     // 7 UART radio TxD

    PORTC.REMAP=((1<<PORT_TC0A_bp)|(1<<PORT_TC0B_bp)) ;
    PORTC.DIR=0xB8;
    //PORTC.OUT=0x03;
    PORTCFG.MPCMASK = 0x03; // Configure several PINxCTRL registers at the same time
	PORTC.PIN0CTRL = (PORTC.PIN0CTRL & ~PORT_OPC_gm) | PORT_OPC_PULLUP_gc; //Enable pull-up to get a defined level on the switches


    ///PORT D
    // 0 Radio set/reset  4 Sim900 PWR key
    // 1 Sim900 reset     5 SPI 1
    // 2 Sim900 RxD       6 SPI 2
    // 3 Sim900 TxD       7 SPI 3
    PORTD.DIR=0x35;
    PORTD.OUT=0x01;

    /// PORT E
    // 0 DC/DC 4v3        2 RPI RxD
    // 1 RPI 3v3          3 RPI TxD
    PORTE.DIR=0x0B;
    PORTE.OUT=0x03;

    /// PORT R
    // 0 RPI 4v3
    // 1 MOSTEK H Stand by
    PORTR.DIR=0x03;
    PORTR.OUT=0x00;


    //Timer 0 H bridge PWM
    TCC0.PER   = 100;   //Zakres

    ///LoadConfig overrides this register
    TCC0.CTRLA = 0x07;      //Preskaler 1024 PWM @ 0.32 KHz Za niska częstotliwość
    //TCC0.CTRLA = 0x06;    //Preskaler 256  PWM @ 1.28 kHz
    //TCC0.CTRLA = 0x05;    //Preskaler 64 Silniki nie chcą startować @  5 kHz
    //TCC0.CTRLA = 0x04;    //Preskaler 8 Silniki nie chcą startować   @ 40 kHz


    //TCC0.CTRLA = 0x04;    //Preskaler 8

    TCC0.CTRLB = 0x33;  //A and B channels enabled, single slope
    TCC0.CTRLC = 0x00;



    TCC0.CCAH = 0x00;
    TCC0.CCBH = 0x00;
    //PORTC.OUTSET = 0x30;

    /// A/C init
    //ADCA.CTRLA     = ADC_ENABLE_bm | ADC_CH0START_bm;        //Włączenie przetwornika AC oraz uruchomienie pomiaru na kanale 0
    //ADCA.CTRLB     = 0x80;//ADC_LOWIMP_bm;                            //Sprawdzam, czy poprawi się jakość pomiaru
    ADCA.REFCTRL   = ADC_BANDGAP_bm | ADC_TEMPREF_bm | ADC_REFSEL_INTVCC_gc;          //BANDGAP enable, TempRef enable, Vref = VCC/1.6 V
    ADCA.EVCTRL    = 0;
    ADCA.PRESCALER = ADC_PRESCALER_DIV256_gc;                  //prescaler 256, f=125kHz
    ADCA.INTFLAGS  = 0x0F;

    ADCA.CALL = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0) );
    ADCA.CALH = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1) );

    TwiMaster_Init(&hardwarePAL.twiSensors, &TWIC, TWI_MASTER_INTLVL_LO_gc, TWI_BAUDSETTING);

    //TWI_MasterWriteRead(&hardwarePAL.twiSensors, 10, NULL, 1, 1);
}

void offHbridge()
{
    PORTR.OUTCLR = 0x02;

    PORTA.OUTCLR = 0xF0;
}

void forwardA(uint8_t left, uint8_t right)
{
    PORTR.OUTSET = 0x02;
    PORTA.OUTCLR = 0x60;
    PORTA.OUTSET = 0x90;

    TCC0.CCA = left;
    TCC0.CCB = right;
}


void backwordA(uint8_t left, uint8_t right)
{
    PORTR.OUTSET = 0x02;
    PORTA.OUTCLR = 0x90;
    PORTA.OUTSET = 0x60;

    TCC0.CCA = left;
    TCC0.CCB = right;
}

void rotateLeftA(uint8_t left, uint8_t right)
{
    PORTR.OUTSET = 0x02;
    PORTA.OUTCLR = 0xA0;
    PORTA.OUTSET = 0x50;

    TCC0.CCA = left;
    TCC0.CCB = right;
}

void rotateRightA(uint8_t left, uint8_t right)
{
    PORTR.OUTSET = 0x02;

    PORTA.OUTCLR = 0x50;
    PORTA.OUTSET = 0xA0;

    TCC0.CCA = left;
    TCC0.CCB = right;
}

//0 - (18)        4 - B1 in
//1 - (17)        5 - A1 in
//2 - (20)        6 - B2 in
//3 - (19)        7 - A2 in
void forwardB(uint8_t left, uint8_t right)
{
    //PORTR.OUTSET = 0x02;
    PORTA.OUTSET = 0xC0;
    PORTA.OUTCLR = 0x30;

    TCC0.CCA = left;
    TCC0.CCB = right;
}


void backwordB(uint8_t left, uint8_t right)
{
    //PORTR.OUTSET = 0x02;
    PORTA.OUTCLR = 0xC0;
    PORTA.OUTSET = 0x30;

    TCC0.CCA = left;
    TCC0.CCB = right;
}

void rotateLeftB(uint8_t left, uint8_t right)
{
    // PORTR.OUTSET = 0x02;
    PORTA.OUTCLR = 0x90;
    PORTA.OUTSET = 0x60;

    TCC0.CCA = left;
    TCC0.CCB = right;
}

void rotateRightB(uint8_t left, uint8_t right)
{
    // PORTR.OUTSET = 0x02;
    PORTA.OUTSET = 0x90;
    PORTA.OUTCLR = 0x60;

    TCC0.CCA = left;
    TCC0.CCB = right;
}

void pwrOn4v3()
{
    PORTE.OUTCLR = 0x01;
}

void pwrOn3v3rpi(void)
{
    PORTE.OUTCLR = 0x02;
}

void pwrOn4v3rpi(void)
{
    PORTE.OUTCLR = 0x01;
    PORTR.OUTSET = 0x01;
}

void pwrOff4v3()
{
    PORTE.OUTSET = 0x01;
}


void pwrOff3v3rpi(void)
{
    PORTE.OUTSET = 0x02;
}

void pwrOff4v3rpi(void)
{
    PORTR.OUTCLR = 0x01;
}

uint8_t isPwr4v3(void)
{
    return ((PORTE.OUT & 0x01) == 0x00);
}

uint8_t isPwr3v3rpi(void)
{
    return ((PORTE.OUT & 0x02) == 0x00);
}

uint8_t isPwr4v3rpi(void)
{
//    return ((PORTE.OUT & 0x01 == 0x00) && ((PORTR.OUT & 0x01) == 0x01));
    return (PORTR.OUT & 0x01);
}

ISR(TWIC_TWIM_vect)
{
    hardwarePAL.twiSensors.hptw = pdFALSE;
    TwiMaster_Irq(&hardwarePAL.twiSensors);
    if (hardwarePAL.twiSensors.hptw != pdFALSE)
        taskYIELD();
}

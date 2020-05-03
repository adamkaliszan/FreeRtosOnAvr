#include <stdint.h>

#include "include/adxl345.h"

static void adxl345_writeRegister8(uint8_t reg, uint8_t value);
static uint8_t adxl345_readRegister8(uint8_t reg);
static uint8_t adxl345_fastRegister8(uint8_t reg);
static int16_t adxl345_readRegister16(uint8_t reg);
static void adxl345_writeRegisterBit(uint8_t reg, uint8_t pos, uint8_t state);
static uint8_t adxl345_readRegisterBit(uint8_t reg, uint8_t pos);


static void beginFun()
{
    
}

static void beginTransmissionFun(uint8_t address)
{
    
}

static void sendFun(uint8_t address)
{
    
}

static void endTransmissionFun(void)
{
    
}

static void requestFromFun(uint8_t address, uint8_t len)
{
    
}

static uint8_t receiveFun(void)
{
    return 0;
}

static uint8_t availableFun(void)
{
    return 0;
}

struct
{
    void (*begin)(void);
    void (*beginTransmission)(uint8_t address);
    void (*send)(uint8_t value);
    void (*endTransmission)(void);
    
    void (*requestFrom)(uint8_t address, uint8_t len);
    uint8_t (*receive)(void);
    uint8_t (*available)(void);
} Wire = {
    .begin = beginFun,
    .beginTransmission = beginTransmissionFun,
    .send = sendFun,
    .endTransmission = endTransmissionFun,
    .requestFrom = requestFromFun,
    .receive = receiveFun,
    .available = availableFun
};

float constrain (float x, float min, float max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

uint8_t adxl345_begin()
{
    adxl345_f.XAxis = 0;
    adxl345_f.YAxis = 0;
    adxl345_f.ZAxis = 0;

    Wire.begin();

    // Check ADXL345 REG DEVID
    if (adxl345_fastRegister8(ADXL345_REG_DEVID) != 0xE5)
    {
        return 0;
    }

    // Enable measurement mode (0b00001000)
    adxl345_writeRegister8(ADXL345_REG_POWER_CTL, 0x08);

    adxl345_clearSettings();

    return 1;
}

// Set Range
void adxl345_setRange(adxl345_range_t range)
{
  // Get actual value register
  uint8_t value = adxl345_readRegister8(ADXL345_REG_DATA_FORMAT);

  // Update the data rate
  // (&) 0b11110000 (0xF0 - Leave HSB)
  // (|) 0b0000xx?? (range - Set range)
  // (|) 0b00001000 (0x08 - Set Full Res)
  value &= 0xF0;
  value |= range;
  value |= 0x08;

  adxl345_writeRegister8(ADXL345_REG_DATA_FORMAT, value);
}

// Get Range
adxl345_range_t adxl345_getRange(void)
{
    return (adxl345_range_t)(adxl345_readRegister8(ADXL345_REG_DATA_FORMAT) & 0x03);
}

// Set Data Rate
void adxl345_setDataRate(adxl345_dataRate_t dataRate)
{
    adxl345_writeRegister8(ADXL345_REG_BW_RATE, dataRate);
}

// Get Data Rate
adxl345_dataRate_t adxl345_getDataRate(void)
{
    return (adxl345_dataRate_t)(adxl345_readRegister8(ADXL345_REG_BW_RATE) & 0x0F);
}

// Low Pass Filter
struct Vector adxl345_lowPassFilter(struct Vector *vector, float alpha)
{
    adxl345_f.XAxis = vector->XAxis * alpha + (adxl345_f.XAxis * (1.0 - alpha));
    adxl345_f.YAxis = vector->YAxis * alpha + (adxl345_f.YAxis * (1.0 - alpha));
    adxl345_f.ZAxis = vector->ZAxis * alpha + (adxl345_f.ZAxis * (1.0 - alpha));
    return adxl345_f;
}

// Read raw values
struct Vector adxl345_readRaw(void)
{
    adxl345_r.XAxis = adxl345_readRegister16(ADXL345_REG_DATAX0);
    adxl345_r.YAxis = adxl345_readRegister16(ADXL345_REG_DATAY0);
    adxl345_r.ZAxis = adxl345_readRegister16(ADXL345_REG_DATAZ0);
    return adxl345_r;
}

// Read normalized values
struct Vector adxl345_readNormalize(float gravityFactor)
{
    adxl345_readRaw();

    // (4 mg/LSB scale factor in Full Res) * gravity factor
    adxl345_n.XAxis = adxl345_r.XAxis * 0.004 * gravityFactor;
    adxl345_n.YAxis = adxl345_r.YAxis * 0.004 * gravityFactor;
    adxl345_n.ZAxis = adxl345_r.ZAxis * 0.004 * gravityFactor;

    return adxl345_n;
}

// Read scaled values
struct Vector adxl345_readScaled(void)
{
    adxl345_readRaw();

    // (4 mg/LSB scale factor in Full Res)
    adxl345_n.XAxis = adxl345_r.XAxis * 0.004;
    adxl345_n.YAxis = adxl345_r.YAxis * 0.004;
    adxl345_n.ZAxis = adxl345_r.ZAxis * 0.004;

    return adxl345_n;
}

void adxl345_clearSettings(void)
{
    adxl345_setRange(ADXL345_RANGE_2G);
    adxl345_setDataRate(ADXL345_DATARATE_100HZ);

    adxl345_writeRegister8(ADXL345_REG_THRESH_TAP, 0x00);
    adxl345_writeRegister8(ADXL345_REG_DUR, 0x00);
    adxl345_writeRegister8(ADXL345_REG_LATENT, 0x00);
    adxl345_writeRegister8(ADXL345_REG_WINDOW, 0x00);
    adxl345_writeRegister8(ADXL345_REG_THRESH_ACT, 0x00);
    adxl345_writeRegister8(ADXL345_REG_THRESH_INACT, 0x00);
    adxl345_writeRegister8(ADXL345_REG_TIME_INACT, 0x00);
    adxl345_writeRegister8(ADXL345_REG_THRESH_FF, 0x00);
    adxl345_writeRegister8(ADXL345_REG_TIME_FF, 0x00);

    uint8_t value;

    value = adxl345_readRegister8(ADXL345_REG_ACT_INACT_CTL);
    value &= 0b10001000;
    adxl345_writeRegister8(ADXL345_REG_ACT_INACT_CTL, value);

    value = adxl345_readRegister8(ADXL345_REG_TAP_AXES);
    value &= 0b11111000;
    adxl345_writeRegister8(ADXL345_REG_TAP_AXES, value);
}

// Set Tap Threshold (62.5mg / LSB)
void adxl345_setTapThreshold(float threshold)
{
    uint8_t scaled = constrain(threshold / 0.0625f, 0, 255);
    adxl345_writeRegister8(ADXL345_REG_THRESH_TAP, scaled);
}

// Get Tap Threshold (62.5mg / LSB)
float adxl345_getTapThreshold(void)
{
    return adxl345_readRegister8(ADXL345_REG_THRESH_TAP) * 0.0625f;
}

// Set Tap Duration (625us / LSB)
void adxl345_setTapDuration(float duration)
{
    uint8_t scaled = constrain(duration / 0.000625f, 0, 255);
    adxl345_writeRegister8(ADXL345_REG_DUR, scaled);
}

// Get Tap Duration (625us / LSB)
float adxl345_getTapDuration(void)
{
    return adxl345_readRegister8(ADXL345_REG_DUR) * 0.000625f;
}

// Set Double Tap Latency (1.25ms / LSB)
void setDoubleTapLatency(float latency)
{
    uint8_t scaled = constrain(latency / 0.00125f, 0, 255);
    adxl345_writeRegister8(ADXL345_REG_LATENT, scaled);
}

// Get Double Tap Latency (1.25ms / LSB)
float adxl345_getDoubleTapLatency()
{
    return adxl345_readRegister8(ADXL345_REG_LATENT) * 0.00125f;
}

// Set Double Tap Window (1.25ms / LSB)
void adxl345_setDoubleTapWindow(float window)
{
    uint8_t scaled = constrain(window / 0.00125f, 0, 255);
    adxl345_writeRegister8(ADXL345_REG_WINDOW, scaled);
}

// Get Double Tap Window (1.25ms / LSB)
float adxl345_getDoubleTapWindow(void)
{
    return adxl345_readRegister8(ADXL345_REG_WINDOW) * 0.00125f;
}

// Set Activity Threshold (62.5mg / LSB)
void adxl345_setActivityThreshold(float threshold)
{
    uint8_t scaled = constrain(threshold / 0.0625f, 0, 255);
    adxl345_writeRegister8(ADXL345_REG_THRESH_ACT, scaled);
}

// Get Activity Threshold (65.5mg / LSB)
float adxl345_getActivityThreshold(void)
{
    return adxl345_readRegister8(ADXL345_REG_THRESH_ACT) * 0.0625f;
}

// Set Inactivity Threshold (65.5mg / LSB)
void adxl345_setInactivityThreshold(float threshold)
{
    uint8_t scaled = constrain(threshold / 0.0625f, 0, 255);
    adxl345_writeRegister8(ADXL345_REG_THRESH_INACT, scaled);
}

// Get Incactivity Threshold (65.5mg / LSB)
float adxl345_getInactivityThreshold(void)
{
    return adxl345_readRegister8(ADXL345_REG_THRESH_INACT) * 0.0625f;
}

// Set Inactivity Time (s / LSB)
void adxl345_setTimeInactivity(uint8_t time)
{
    adxl345_writeRegister8(ADXL345_REG_TIME_INACT, time);
}

// Get Inactivity Time (s / LSB)
uint8_t adxl345_getTimeInactivity(void)
{
    return adxl345_readRegister8(ADXL345_REG_TIME_INACT);
}

// Set Free Fall Threshold (65.5mg / LSB)
void adxl345_setFreeFallThreshold(float threshold)
{
    uint8_t scaled = constrain(threshold / 0.0625f, 0, 255);
    adxl345_writeRegister8(ADXL345_REG_THRESH_FF, scaled);
}

// Get Free Fall Threshold (65.5mg / LSB)
float adxl345_getFreeFallThreshold(void)
{
    return adxl345_readRegister8(ADXL345_REG_THRESH_FF) * 0.0625f;
}

// Set Free Fall Duratiom (5ms / LSB)
void adxl345_setFreeFallDuration(float duration)
{
    uint8_t scaled = constrain(duration / 0.005f, 0, 255);
    adxl345_writeRegister8(ADXL345_REG_TIME_FF, scaled);
}

// Get Free Fall Duratiom
float adxl345_getFreeFallDuration()
{
    return adxl345_readRegister8(ADXL345_REG_TIME_FF) * 0.005f;
}

void adxl345_setActivityX(uint8_t state)
{
    adxl345_writeRegisterBit(ADXL345_REG_ACT_INACT_CTL, 6, state);
}

uint8_t adxl345_getActivityX(void)
{
    return adxl345_readRegisterBit(ADXL345_REG_ACT_INACT_CTL, 6);
}

void adxl345_setActivityY(uint8_t state)
{
    adxl345_writeRegisterBit(ADXL345_REG_ACT_INACT_CTL, 5, state);
}

uint8_t adxl345_getActivityY(void)
{
    return adxl345_readRegisterBit(ADXL345_REG_ACT_INACT_CTL, 5);
}

void adxl345_setActivityZ(uint8_t state)
{
    adxl345_writeRegisterBit(ADXL345_REG_ACT_INACT_CTL, 4, state);
}

uint8_t adxl345_getActivityZ(void)
{
    return adxl345_readRegisterBit(ADXL345_REG_ACT_INACT_CTL, 4);
}

void adxl345_setActivityXYZ(uint8_t state)
{
    uint8_t value;

    value = adxl345_readRegister8(ADXL345_REG_ACT_INACT_CTL);

    if (state)
    	value |= 0b00111000;
    else
        value &= 0b11000111;

    adxl345_writeRegister8(ADXL345_REG_ACT_INACT_CTL, value);
}


void adxl345_setInactivityX(uint8_t state) 
{
    adxl345_writeRegisterBit(ADXL345_REG_ACT_INACT_CTL, 2, state);
}

uint8_t adxl345_getInactivityX(void)
{
    return adxl345_readRegisterBit(ADXL345_REG_ACT_INACT_CTL, 2);
}

void adxl345_setInactivityY(uint8_t state)
{
    adxl345_writeRegisterBit(ADXL345_REG_ACT_INACT_CTL, 1, state);
}

uint8_t adxl345_getInactivityY(void)
{
    return adxl345_readRegisterBit(ADXL345_REG_ACT_INACT_CTL, 1);
}

void adxl345_setInactivityZ(uint8_t state)
{
    adxl345_writeRegisterBit(ADXL345_REG_ACT_INACT_CTL, 0, state);
}

uint8_t adxl345_getInactivityZ(void)
{
    return adxl345_readRegisterBit(ADXL345_REG_ACT_INACT_CTL, 0);
}

void adxl345_setInactivityXYZ(uint8_t state)
{
    uint8_t value;

    value = adxl345_readRegister8(ADXL345_REG_ACT_INACT_CTL);

    if (state)
    	value |= 0b00000111;
    else
    	value &= 0b11111000;

    adxl345_writeRegister8(ADXL345_REG_ACT_INACT_CTL, value);
}

void adxl345_setTapDetectionX(uint8_t state)
{
    adxl345_writeRegisterBit(ADXL345_REG_TAP_AXES, 2, state);
}

uint8_t adxl345_getTapDetectionX(void)
{
    return adxl345_readRegisterBit(ADXL345_REG_TAP_AXES, 2);
}

void adxl345_setTapDetectionY(uint8_t state)
{
    adxl345_writeRegisterBit(ADXL345_REG_TAP_AXES, 1, state);
}

uint8_t adxl345_getTapDetectionY(void)
{
    return adxl345_readRegisterBit(ADXL345_REG_TAP_AXES, 1);
}

void adxl345_setTapDetectionZ(uint8_t state)
{
    adxl345_writeRegisterBit(ADXL345_REG_TAP_AXES, 0, state);
}

uint8_t adxl345_getTapDetectionZ(void)
{
    return adxl345_readRegisterBit(ADXL345_REG_TAP_AXES, 0);
}

void adxl345_setTapDetectionXYZ(uint8_t state)
{
    uint8_t value;

    value = adxl345_readRegister8(ADXL345_REG_TAP_AXES);

    if (state)
    	value |= 0b00000111;
    else
    	value &= 0b11111000;

    adxl345_writeRegister8(ADXL345_REG_TAP_AXES, value);
}


void adxl345_useInterrupt(adxl345_int_t interrupt)
{
    if (interrupt == 0)
    {
    	adxl345_writeRegister8(ADXL345_REG_INT_MAP, 0x00);
    } 
    else
    {
    	adxl345_writeRegister8(ADXL345_REG_INT_MAP, 0xFF);
    }

    adxl345_writeRegister8(ADXL345_REG_INT_ENABLE, 0xFF);
}

struct Activites adxl345_readActivites(void)
{
    uint8_t data = adxl345_readRegister8(ADXL345_REG_INT_SOURCE);
    adxl345_a.data1 = data;
    //adxl345_a.isOverrun = ((data >> ADXL345_OVERRUN) & 1);
    //adxl345_a.isWatermark = ((data >> ADXL345_WATERMARK) & 1);
    //adxl345_a.isFreeFall = ((data >> ADXL345_FREE_FALL) & 1);
    //adxl345_a.isInactivity = ((data >> ADXL345_INACTIVITY) & 1);
    //adxl345_a.isActivity = ((data >> ADXL345_ACTIVITY) & 1);
    //adxl345_a.isDoubleTap = ((data >> ADXL345_DOUBLE_TAP) & 1);
    //adxl345_a.isTap = ((data >> ADXL345_SINGLE_TAP) & 1);
    //adxl345_a.isDataReady = ((data >> ADXL345_DATA_READY) & 1);

    data = adxl345_readRegister8(ADXL345_REG_ACT_TAP_STATUS);
    adxl345_a.data2 = data;

    //adxl345_a.isActivityOnX = ((data >> 6) & 1);
    //adxl345_a.isActivityOnY = ((data >> 5) & 1);
    //adxl345_a.isActivityOnZ = ((data >> 4) & 1);
    //adxl345_a.isTapOnX = ((data >> 2) & 1);
    //adxl345_a.isTapOnY = ((data >> 1) & 1);
    //adxl345_a.isTapOnZ = ((data >> 0) & 1);

    return adxl345_a;
}

// Write byte to register
void adxl345_writeRegister8(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.send(reg);
    Wire.send(value);
    Wire.endTransmission();
}

// Read byte to register
uint8_t adxl345_fastRegister8(uint8_t reg)
{
    uint8_t value;
    Wire.beginTransmission(ADXL345_ADDRESS);
    #if ARDUINO >= 100
        Wire.write(reg);
    #else
        Wire.send(reg);
    #endif
    Wire.endTransmission();

    Wire.requestFrom(ADXL345_ADDRESS, 1);
    value = Wire.receive();
    Wire.endTransmission();

    return value;
}

// Read byte from register
uint8_t adxl345_readRegister8(uint8_t reg)
{
    uint8_t value;
    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.send(reg);
    Wire.endTransmission();

    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.requestFrom(ADXL345_ADDRESS, 1);
    while(!Wire.available()) {};
        value = Wire.receive();
    Wire.endTransmission();

    return value;
}

// Read word from register
int16_t adxl345_readRegister16(uint8_t reg)
{
    int16_t value;
    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.send(reg);
    Wire.endTransmission();

    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.requestFrom(ADXL345_ADDRESS, 2);
    while(!Wire.available()) {};
        uint8_t vla = Wire.receive();
        uint8_t vha = Wire.receive();

    Wire.endTransmission();

    value = vha << 8 | vla;

    return value;
}

void adxl345_writeRegisterBit(uint8_t reg, uint8_t pos, uint8_t state)
{
    uint8_t value;
    value = adxl345_readRegister8(reg);

    if (state)
    {
	value |= (1 << pos);
    } else 
    {
	value &= ~(1 << pos);
    }

    adxl345_writeRegister8(reg, value);
}

uint8_t adxl345_readRegisterBit(uint8_t reg, uint8_t pos)
{
    uint8_t value;
    value = adxl345_readRegister8(reg);
    return ((value >> pos) & 1);
}

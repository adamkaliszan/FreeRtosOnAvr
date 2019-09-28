#include <FreeRTOS.h>


typedef struct SensorsData
{
    uint16_t frameNo;
} SensorsData_t;


typedef struct SensorsHandler
{
    xSemaphoreHandle mutex;

    uint8_t wrIdx;
    SensorsData_t data[2];
} SensorsHandler_t;


void sensorsInit();

void sensorsUpdate();

void sensorsReadData();
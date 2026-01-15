#pragma once
#include "Arduino.h"

class resourceLock
{
#if USE_RTOS
public:
    resourceLock() { mutex = xSemaphoreCreateMutex(); }
#endif

public:
    void lock();
    void unlock();

private:
#if USE_RTOS
    SemaphoreHandle_t mutex;
#endif
};
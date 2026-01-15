#include "resourceLock.h"

#if USE_RTOS

void resourceLock::lock()
{
    xSemaphoreTake(mutex, portMAX_DELAY);
}

void resourceLock::unlock()
{
    xSemaphoreGive(mutex);
}

#else // no RTOS, no locks needed

void resourceLock::lock() {}

void resourceLock::unlock() {}

#endif

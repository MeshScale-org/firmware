#pragma once
#include "Arduino.h"

class thread
{
public:
    thread(const char *name) : _name(name) {}

public:
#ifdef USE_RTOS
    static void runThread(void *thisThread)
    {
        thread *whatThread = static_cast<thread *>(thisThread);
        while (true)
        {
            whatThread->runOnce();
            if (whatThread->nextRunTime == 0)
            {
                vTaskSuspend(NULL); // suspend self
            }
            vTaskDelay(whatThread->nextRunTime - millis());
        }
    }
#endif
    const char *getName() { return _name; }

private:
    // called by scheduler
    virtual void runOnce() = 0;
    const char *_name;

protected:
    // next requested run time in millis, 0 to never call again
    unsigned long nextRunTime = 1; // set initial value to 1 so every thread is started on first run

    friend class scheduler;
};
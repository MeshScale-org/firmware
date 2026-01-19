#pragma once

#include <Arduino.h>
#include <vector>
#include "thread.h"

// scheduler singleton
class scheduler
{
public:
#ifndef USE_RTOS
    // when running in cooperative threading mode (no RTOS)
    static void runCoOp() { get().runCoOpImpl(); };
#endif
    static void addThread(thread *newThread) { get().addThreadImpl(newThread); };

    // smallest timeTilNextRun of all threads: When should the scheduler be called again (or earlier), can be used for sleep/power saving
    static unsigned long minTimeTilNextRun;

private:
    // get singleton instance
    static scheduler &get()
    {
        static scheduler instance;
        return instance;
    }

#ifndef USE_RTOS
    void runCoOpImpl();
#endif
    void addThreadImpl(thread *newThread);

private:
#if USE_RTOS
    std::vector<TaskHandle_t *> TaskHandles; // stored but not used
#else
    std::vector<thread *> threads;
#endif
};

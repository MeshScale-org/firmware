#include "scheduler.h"

void scheduler::runCoOpImpl()
{
#ifndef USE_RTOS
    for (uint16_t i = 0; i < threads.size(); i++)
    {
        if (threads[i]->nextRunTime && threads[i]->nextRunTime <= millis())
        {
            threads[i]->runOnce();
            if (threads[i]->nextRunTime && threads[i]->nextRunTime <= firstNextRunTime)
            {
                firstNextRunTime = threads[i]->nextRunTime;
            }
        }
    }
#endif
}

void scheduler::registerTaskImpl(thread *newThread)
{
#ifdef USE_RTOS
// TODO
#else
    threads.push_back(newThread);
#endif
}
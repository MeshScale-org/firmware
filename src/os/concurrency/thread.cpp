#include "thread.h"

#include "os/concurrency/scheduler.h"

#ifdef USE_RTOS
void thread::runThread(void *thisThread)
{
    thread *whatThread = static_cast<thread *>(thisThread);
    while (true)
    {
        whatThread->runOnce();
        if (whatThread->timeTilNextRun == 0)
        {
            Serial.printf("runThread: %s: suspending\n", whatThread->getName().c_str());
            vTaskSuspend(NULL); // suspend self
        }
        if (whatThread->timeTilNextRun < scheduler::minTimeTilNextRun)
        {
            scheduler::minTimeTilNextRun = whatThread->timeTilNextRun;
        }
        vTaskDelay(pdMS_TO_TICKS(whatThread->timeTilNextRun));
    }
}
#endif

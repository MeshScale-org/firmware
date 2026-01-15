#include "scheduler.h"

#ifndef USE_RTOS
void scheduler::runCoOpImpl()
{
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
}
#endif

void scheduler::registerTaskImpl(thread *newThread)
{
#ifdef USE_RTOS // use freeRTOS

    TaskHandle_t *newTaskHandle = nullptr;

#ifdef MCU_ESP32 // ESP-IDF freeRTOS

    //---taskUicontroller
    xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
        thread::runThread,    // Function to be called
        newThread->getName(), // Name of task
        8192,                 // Stack size (bytes in ESP32, words in FreeRTOS)
        newThread,            // Parameter to pass to function
        5,                    // Task priority (0 to configMAX_PRIORITIES - 1) (default 0 to 24)
        newTaskHandle,        // Task handle
        tskNO_AFFINITY);      // Select core or tskNO_AFFINITY (ESP32 only)

#else // vanilla freeRTOS
    // TODO
    Serial.printf("ERROR: vanilla freeRTOS not supported");

#endif // MCU_ESP32

    TaskHandles.push_back(newTaskHandle);
    Serial.printf("Created RTOS task: %s\n", newThread->getName());
#else  // CoOp scheduler
    threads.push_back(newThread);
    Serial.printf("Created CoOp task: %s\n", newThread->getName());
#endif // USE_RTOS
}
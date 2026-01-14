#pragma once

#include <Arduino.h>
#include <vector>
#include "thread.h"

// scheduler singleton
class scheduler
{
public:
    // when running in cooperative threading mode (no RTOS)
    static void runCoOp() { get().runCoOpImpl(); };
    static void registerTask(thread *newThread) { get().registerTaskImpl(newThread); };
    // when should the scheduler be called again (or earlier)
    unsigned long firstNextRunTime;

private:
    // get singleton instance
    static scheduler &get()
    {
        static scheduler instance;
        return instance;
    }
    void runCoOpImpl();
    void registerTaskImpl(thread *newThread);

private:
    std::vector<thread *> threads;
};

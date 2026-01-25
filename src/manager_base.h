#pragma once
#include "rumor.h"
#include "os/concurrency/resourceLock.h"
#include <mutex>
#include <queue>

class manager_base
{
public:
    manager_base() {};

public:
    // return requested timeTilNextRun
    virtual unsigned long loop() = 0;
    virtual void takeRumor(rumor_t newRumor)
    {
        std::lock_guard<resourceLock> lg(rumorsInMutex);
        rumorsIn.push(newRumor);
    };

protected:
    resourceLock rumorsInMutex;
    std::queue<rumor_t> rumorsIn;
};

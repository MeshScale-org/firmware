#pragma once
#include "rumor.h"
#include "os/concurrency/queueL.h"

class manager_base
{
public:
    manager_base() {};

public:
    // return requested timeTilNextRun
    virtual unsigned long loop() = 0;
    virtual void takeRumor(rumor_t newRumor)
    {
        rumorsIn.push(newRumor);
    };

protected:
    queueL<rumor_t> rumorsIn;
};

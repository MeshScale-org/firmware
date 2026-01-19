#pragma once
#include "rumor.h"

class manager_base
{
public:
    manager_base() {};

public:
    // return requested timeTilNextRun
    virtual unsigned long loop() = 0;
    virtual void takeRumor(rumor_t newRumor) = 0;

private:
};

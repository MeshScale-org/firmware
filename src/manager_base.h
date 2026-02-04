#pragma once
#include "protocolBuffers/generated/messageTest.pb.h"
#include "os/concurrency/queueL.h"

class manager_base
{
public:
    manager_base() {};

public:
    // return requested timeTilNextRun
    virtual unsigned long loop() = 0;
    virtual void takeRumor(meshScale_PbMessage newRumor)
    {
        rumorsIn.push(newRumor);
        Serial.printf("manager_base: added rewRumor\n");
    };

protected:
    queueL<meshScale_PbMessage> rumorsIn;
};

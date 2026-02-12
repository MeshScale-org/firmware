#pragma once
#include "protocolBuffers/generated/PbMessage.pb.h"
#include "os/concurrency/queueL.h"

class manager_base
{
public:
    manager_base() {};

public:
    // return requested timeTilNextRun
    virtual unsigned long loop() = 0;
    virtual void takePbMessage(meshScale_PbMessage newPbMessage)
    {
        pbMessagesIn.push(newPbMessage);
        Serial.printf("manager_base: added newPbMessage\n");
    };

protected:
    queueL<meshScale_PbMessage> pbMessagesIn;
};

#pragma once
#include "protocolBuffers/generated/PbMessage.pb.h"
#include "os/concurrency/queueL.h"

class manager_base
{
public:
    manager_base() {};

public:
    // setup can be implemented, all setups are run by scheduler before loop threads are created
    virtual void setup() {};
    // return requested timeTilNextRun
    virtual unsigned long loop() = 0;
    // All managers have a message queue
    virtual void takePbMessage(meshScale_PbMessage newPbMessage)
    {
        pbMessagesIn.push(newPbMessage);
    };

protected:
    queueL<meshScale_PbMessage> pbMessagesIn;
};

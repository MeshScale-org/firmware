#pragma once
#include "protocolBuffers/generated/PbMessage.pb.h"
#include "os/concurrency/queueL.h"
#include <mutex>
#include <memory>

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
    virtual void takePbMessage(std::shared_ptr<const meshScale_PbMessage> newPbMessage)
    {
        std::lock_guard<resourceLock> lg(pbMessagesIn);
        // pop the oldest message to avoid memory leak, messages can be lost if the manager can't keep up
        while (pbMessagesIn.size() > maxQueueSize)
        {
            Serial.printf("manager_base::takePbMessage: Warning: pbMessagesIn at max size: pop() oldest\n");
            pbMessagesIn.pop();
        }
        pbMessagesIn.push(newPbMessage);
    };

protected:
    queueL<std::shared_ptr<const meshScale_PbMessage>> pbMessagesIn;
    uint8_t maxQueueSize = 10;
};

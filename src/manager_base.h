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
    // return requested timeTilNextRun
    virtual unsigned long loop() = 0;

    // Add new pbMessage to our queue
    virtual void takePbMessage(std::shared_ptr<const meshScale_PbMessage> newPbMessage);

    /*
     * Handle pbMessages
     * Switch depending on the type of pbMessage
     * if a manager wants to do anything with that type, it should override the function in manager_base
     */
protected:
    // Handle the pbMessages in our queue
    virtual void handlePbMessages();

    /*
     * Instruction
     */
    virtual void handleInstruction(meshScale_Instruction instruction);
    /*
     *specific instruction type
     * Override in specific manager. Default: ignore
     */
    virtual void handleGetReq(meshScale_Instruction_GetReq &getReq, meshScale_Source &source) {};
    virtual void handleGetResp(meshScale_Instruction_GetResp &getResp, meshScale_Source &source) {};
    virtual void handleSetReq(meshScale_Instruction_SetReq &setReq, meshScale_Source &source) {};
    virtual void handleSetResp(meshScale_Instruction_SetResp &setResp, meshScale_Source &source) {};
    virtual void handleExecReq(meshScale_Instruction_ExecReq &execReq, meshScale_Source &source) {};
    virtual void handleExecResp(meshScale_Instruction_ExecResp &execResp, meshScale_Source &source) {};

    /*
     *Handle event
     * Overwrite in managers if needed
     */
    virtual void handleEvent(meshScale_Event event) {};

protected:
    queueL<std::shared_ptr<const meshScale_PbMessage>> pbMessagesIn;
    uint8_t maxQueueSize = 10;
};

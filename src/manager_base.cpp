#include "manager_base.h"

// Add new pbMessage to our queue
void manager_base::takePbMessage(std::shared_ptr<const meshScale_PbMessage> newPbMessage)
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

// Handle the pbMessages in our queue
// Switch depending on the type of pbMessage
void manager_base::handlePbMessages()
{
    std::lock_guard<resourceLock> lg(pbMessagesIn);
    while (!pbMessagesIn.empty())
    {
        std::shared_ptr<const meshScale_PbMessage> incoming = pbMessagesIn.front();
        switch (incoming->which_content)
        {
        case meshScale_PbMessage_instruction_tag:
            handleInstruction(incoming->instruction);
            break;
        case meshScale_PbMessage_event_tag:
            handleEvent(incoming->event);
            break;
        default:
            break;
        }
        pbMessagesIn.pop();
    }
}

// Switch depending on the type of instruction message
void manager_base::handleInstruction(meshScale_Instruction instruction)
{
    // only handle when there is a known requester
    if (instruction.has_requester)
    {
        switch (instruction.which_instruction)
        {
        case meshScale_Instruction_get_req_tag:
            handleGetReq(instruction.get_req, instruction.requester);
            break;
        case meshScale_Instruction_get_resp_tag:
            handleGetResp(instruction.get_resp, instruction.requester);
            break;
        case meshScale_Instruction_set_req_tag:
            handleSetReq(instruction.set_req, instruction.requester);
            break;
        case meshScale_Instruction_set_resp_tag:
            handleSetResp(instruction.set_resp, instruction.requester);
            break;
        case meshScale_Instruction_exec_req_tag:
            handleExecReq(instruction.exec_req, instruction.requester);
            break;
        case meshScale_Instruction_exec_resp_tag:
            handleExecResp(instruction.exec_resp, instruction.requester);
            break;
        default:
            // ignore
            break;
        }
    }
    else
    {
        printf("manager_base::handleInstruction: No requester in pbMessage, ignoring ...\n");
    }
};

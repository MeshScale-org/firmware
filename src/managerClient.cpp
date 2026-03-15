#include "managerClient.h"

#include "managerSystem.h"

unsigned long lastAnnounce = 0;
unsigned long announceInterval = 20000;

unsigned long lastPacket = 0;
unsigned long packetInterval = 10000;

void toggleLed()
{
    static bool ledState = 0;
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
}

unsigned long managerClient_t::loop()
{
    if (millis() - lastPacket > packetInterval)
    {
        toggleLed();
        // send packet command to network manager
        std::shared_ptr<meshScale_PbMessage> newMessage = std::make_shared<meshScale_PbMessage>();
        *newMessage = meshScale_PbMessage_init_zero;
        newMessage->which_content = meshScale_PbMessage_instruction_tag;
        newMessage->instruction.which_instruction = meshScale_Instruction_exec_req_tag;
        newMessage->instruction.has_requester = true;
        newMessage->instruction.requester.which_source = meshScale_Source_local_source_tag;
        newMessage->instruction.requester.local_source = meshScale_Source_LocalSource_CLIENT; // pretend for now that the client requests to send a packet
        newMessage->instruction.exec_req.has_command = true;
        newMessage->instruction.exec_req.command.which_proto_module = meshScale_Instruction_Command_chat_commands_tag;
        newMessage->instruction.exec_req.command.chat_commands.which_command = meshScale_Chat_ChatCommands_send_chat_tag;

        std::string dest = "Destination12345"; // should be 16 byte hash
        newMessage->instruction.exec_req.command.chat_commands.send_chat.has_destination_hash = true;
        memcpy(newMessage->instruction.exec_req.command.chat_commands.send_chat.destination_hash.hash, dest.data(), 16);
        strcpy(newMessage->instruction.exec_req.command.chat_commands.send_chat.text_string, "MESSAGE12345");

        managerSystem.takePbMessage(newMessage);

        lastPacket = millis();
    }

    if (millis() - lastAnnounce > announceInterval)
    {
        // send announce command to network manager
        std::shared_ptr<meshScale_PbMessage> newMessage = std::make_shared<meshScale_PbMessage>();
        *newMessage = meshScale_PbMessage_init_zero;
        newMessage->which_content = meshScale_PbMessage_instruction_tag;
        newMessage->instruction.which_instruction = meshScale_Instruction_exec_req_tag;
        newMessage->instruction.has_requester = true;
        newMessage->instruction.requester.which_source = meshScale_Source_local_source_tag;
        newMessage->instruction.requester.local_source = meshScale_Source_LocalSource_CLIENT; // pretend for now that the client requests to do an announce
        newMessage->instruction.exec_req.has_command = true;
        newMessage->instruction.exec_req.command.which_proto_module = meshScale_Instruction_Command_announce_commands_tag;
        newMessage->instruction.exec_req.command.announce_commands.which_command = meshScale_Announce_AnnounceCommands_do_announce_command_tag;
        managerSystem.takePbMessage(newMessage);

        lastAnnounce = millis();
    }
    return clientInterval;
}
managerClient_t &managerClient = managerClient_t::getInstance();

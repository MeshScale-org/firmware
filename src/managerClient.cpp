#include "managerClient.h"
#include "protocolBuffers/generated/PbMessage.pb.h"

#include "managerNetwork.h"

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
        meshScale_PbMessage newMessage = meshScale_PbMessage_init_zero;
        newMessage.has_source = true;
        newMessage.source.which_source = meshScale_Source_local_source_tag;
        newMessage.source.local_source = meshScale_Source_LocalSource_MANAGER_CLIENT;
        newMessage.which_content = meshScale_PbMessage_instruction_tag;
        newMessage.instruction.which_instruction = meshScale_Instruction_exec_req_tag;
        newMessage.instruction.exec_req.has_command = true;
        newMessage.instruction.exec_req.command.which_proto_module = meshScale_Instruction_Command_chat_commands_tag;
        newMessage.instruction.exec_req.command.chat_commands.which_command = meshScale_Chat_ChatCommands_send_chat_tag;

        std::string dest = "someDestination"; // should be 16 byte hash
        newMessage.instruction.exec_req.command.chat_commands.send_chat.has_destination_hash = true;
        memcpy(newMessage.instruction.exec_req.command.chat_commands.send_chat.destination_hash.hash, dest.data(), 16);
        strcpy(newMessage.instruction.exec_req.command.chat_commands.send_chat.text_string, "MESSAGE12345");
        managerNetwork.takePbMessage(newMessage);

        lastPacket = millis();
    }

    if (millis() - lastAnnounce > announceInterval)
    {
        // send announce command to network manager
        meshScale_PbMessage newMessage = meshScale_PbMessage_init_zero;
        newMessage.has_source = true;
        newMessage.source.which_source = meshScale_Source_local_source_tag;
        newMessage.source.local_source = meshScale_Source_LocalSource_MANAGER_CLIENT;
        newMessage.which_content = meshScale_PbMessage_instruction_tag;
        newMessage.instruction.which_instruction = meshScale_Instruction_exec_req_tag;
        newMessage.instruction.exec_req.has_command = true;
        newMessage.instruction.exec_req.command.which_proto_module = meshScale_Instruction_Command_announce_commands_tag;
        newMessage.instruction.exec_req.command.announce_commands.which_command = meshScale_Announce_AnnounceCommands_do_announce_command_tag;
        managerNetwork.takePbMessage(newMessage);

        lastAnnounce = millis();
    }
    return clientInterval;
}
managerClient_t &managerClient = managerClient_t::getInstance();

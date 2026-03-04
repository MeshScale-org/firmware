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
        newMessage.has_timestamp_ms = true;
        newMessage.timestamp_ms = millis();
        newMessage.has_local_source = true;
        newMessage.local_source = meshScale_Source_MANAGER_CLIENT;
        newMessage.which_content = meshScale_PbMessage_chat_action_tag;
        newMessage.content.chat_action.which_action = meshScale_SendChat_chat_tag;
        /*uint8_t dest[16] = {
            1, 2, 3, 4, 5, 6,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        newMessage.content.chat_action.action.send_chat.destination_hash = dest;*/
        /*byte data[] = {
            1, 2, 3, 4, 5, 6,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0};*/
        std::string dest = "someDestination"; // should be 16 byte hash
        memcpy(newMessage.content.chat_action.action.send_chat.destination_hash, dest.data(), 16);
        strcpy(newMessage.content.chat_action.action.send_chat.chat.text_string, "MESSAGE12345");
        managerNetwork.takePbMessage(newMessage);

        lastPacket = millis();
    }

    if (millis() - lastAnnounce > announceInterval)
    {
        // send announce command to network manager
        meshScale_PbMessage newMessage = meshScale_PbMessage_init_zero;
        newMessage.has_timestamp_ms = true;
        newMessage.timestamp_ms = millis();
        newMessage.has_local_source = true;
        newMessage.local_source = meshScale_Source_MANAGER_CLIENT;
        newMessage.which_content = meshScale_PbMessage_announce_action_tag;
        newMessage.content.announce_action.which_action = meshScale_AnnounceAction_single_announce_tag; // actual announce_action.action is not set because it will not be read
        managerNetwork.takePbMessage(newMessage);

        lastAnnounce = millis();
    }
    return clientInterval;
}
managerClient_t &managerClient = managerClient_t::getInstance();

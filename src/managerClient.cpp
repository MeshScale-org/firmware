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
        newMessage.which_content = meshScale_PbMessage_userchat_tag;
        // newMessage.content.userchat.destination_hash = ;
        strcpy(newMessage.content.userchat.text_string, "MESSAGE12345");
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
        newMessage.which_content = meshScale_PbMessage_announce_tag;
        // newMessage.content.announce.destination_hash = ;
        managerNetwork.takePbMessage(newMessage);

        lastAnnounce = millis();
    }
    return clientInterval;
}

managerClient_t &managerClient = managerClient_t::getInstance();

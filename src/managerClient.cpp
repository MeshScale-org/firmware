#include "managerClient.h"
#include "protocolBuffers/generated/messageTest.pb.h"

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
        newMessage.has_timestamp = true;
        newMessage.timestamp = millis();
        newMessage.which_payload = meshScale_PbMessage_command_from_client_tag;
        newMessage.payload.command_from_client.which_command = meshScale_CommandFromClientM_send_text_tag;
        strcpy(newMessage.payload.command_from_client.command.send_text.send_string, "MESSAGE12345");
        newMessage.payload.command_from_client.command.send_text.destination_nr = 7;
        managerNetwork.takeRumor(newMessage);

        lastPacket = millis();
    }

    if (millis() - lastAnnounce > announceInterval)
    {
        // send announce command to network manager
        meshScale_PbMessage newMessage = meshScale_PbMessage_init_zero;
        newMessage.has_timestamp = true;
        newMessage.timestamp = millis();
        newMessage.which_payload = meshScale_PbMessage_command_from_client_tag;
        newMessage.payload.command_from_client.which_command = meshScale_CommandFromClientM_send_announce_tag;
        newMessage.payload.command_from_client.command.send_announce.destination_nr = 18;
        managerNetwork.takeRumor(newMessage);
        lastAnnounce = millis();
    }
    return clientInterval;
}

managerClient_t &managerClient = managerClient_t::getInstance();

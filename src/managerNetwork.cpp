#include "managerNetwork.h"
#include "main.h"

// We initialise two lists of strings to use as app_data
const char *noble_gases[] = {"Helium", "Neon", "Argon", "Krypton", "Xenon", "Radon", "Oganesson"};

void reticulum_announce()
{
    if (destination)
    {
        Serial.printf("Announcing destination...\n");
        // destination.announce(RNS::bytesFromString(fruits[RNS::Cryptography::randomnum() % 7]));
        //  test path
        // destination.announce(RNS::bytesFromString(fruits[RNS::Cryptography::randomnum() % 7]), true, nullptr, RNS::bytesFromString("test_tag"));
        //  test packet send
        destination.announce(RNS::bytesFromString(noble_gases[RNS::Cryptography::randomnum() % 7]));
    }
}

void send_packet()
{
    if (externDestination)
    {
        Serial.println("Creating send packet...");
        RNS::Packet send_packet(externDestination, "msgContent123456");

        Serial.println("Sending send packet...");
        send_packet.pack();
        send_packet.send();
    }
    else
    {
        Serial.println("Not sending package because no destination in known");
    }
}

unsigned long managerNetwork_t::loop()
{
    while (!pbMessagesIn.empty())
    {
        meshScale_PbMessage incoming = pbMessagesIn.front();
        if (incoming.has_timestamp_ms)
        {
            Serial.printf("managerNetwork_t: received pbMessage with timestamp of %d ms ago\n", millis() - incoming.timestamp_ms);
        }
        if (incoming.which_content == meshScale_PbMessage_userchat_tag)
        {

            Serial.printf("managerNetwork_t: received pbMessage to send %s to destination %d\n", incoming.content.userchat.text_string, incoming.content.userchat.destination_hash);
        }
        else if (incoming.which_content == meshScale_PbMessage_announce_tag)
        {
            Serial.printf("managerNetwork_t: received pbMessage to announce destination %d\n", incoming.content.announce.destination_hash);
        }
        else
        {
            Serial.println("unknown pbMessage content type, removing");
        }

        pbMessagesIn.pop();
    }

    // TODO: is this needed since Reticulum::loop() also calls interfaceImpl.loop()
    // handlerNetworkInterfaces is the only handler that calls the networkInterface objects but should stay locked during reticulum.loop because this calls interfaceImplementations directly.
    // not very clean, could be improved
    std::lock_guard<resourceLock> lg(handlerNetworkInterfaces);
    handlerNetworkInterfaces.loop();
    reticulum.loop();

    return reticulumInterval;
};

managerNetwork_t &managerNetwork = managerNetwork_t::getInstance();

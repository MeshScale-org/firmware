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
    while (!rumorsIn.empty())
    {
        meshScale_PbMessage incoming = rumorsIn.front();
        if (incoming.has_timestamp)
        {
            Serial.printf("managerNetwork_t: received rumor with timestamp of %d ms ago\n", millis() - incoming.timestamp);
        }
        if (incoming.which_payload == meshScale_PbMessage_command_from_client_tag)
        {
            if (incoming.payload.command_from_client.which_command == meshScale_CommandFromClientM_send_text_tag)
            {
                Serial.printf("managerNetwork_t: received command from client to send %s to destination nr %d\n", incoming.payload.command_from_client.command.send_text.send_string, incoming.payload.command_from_client.command.send_text.destination_nr);
            }
            else if (incoming.payload.command_from_client.which_command == meshScale_CommandFromClientM_send_announce_tag)
            {
                Serial.printf("managerNetwork_t: received command from client to announce destination nr %d\n", incoming.payload.command_from_client.command.send_announce.destination_nr);
            }
            else
            {
                Serial.println("unknown command from client type");
            }
        }
        else
        {
            Serial.println("unknown payload type");
        }

        rumorsIn.pop();
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

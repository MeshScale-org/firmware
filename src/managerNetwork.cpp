#include "managerNetwork.h"
#include "main.h"

// We initialise two lists of strings to use as app_data
const char *noble_gases[] = {"Helium", "Neon", "Argon", "Krypton", "Xenon", "Radon", "Oganesson"};

void toggleLed()
{
    static bool ledState = 0;
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
}

unsigned long lastAnnounce = 0;
unsigned long announceInterval = 120000;
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

unsigned long lastPacket = 0;
unsigned long packetInterval = 10000;
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
    // lock for limited duration instead of whole scope (lock_guard). Reduce lock time and avoid deadlock
    handlerNetworkInterfaces.lock();
    handlerNetworkInterfaces.loop();
    handlerNetworkInterfaces.unlock();

    reticulum.loop();

    if (millis() - lastPacket > packetInterval)
    {
        toggleLed();
        send_packet();
        lastPacket = millis();
    }

    if (millis() - lastAnnounce > announceInterval)
    {
        reticulum_announce();
        lastAnnounce = millis();
    }

    return reticulumInterval;
};

managerNetwork_t &managerNetwork = managerNetwork_t::getInstance();
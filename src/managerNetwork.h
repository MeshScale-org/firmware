#pragma once
#include "manager_base.h"
#include "Reticulum.h"

/*
The network Manager

Jobs:


    ...
*/

class managerNetwork_t : public manager_base
{
public:
    managerNetwork_t(managerNetwork_t &) = delete;
    // get singleton instance
    inline static managerNetwork_t &getInstance()
    {
        static managerNetwork_t instance;
        return instance;
    };

private:
    managerNetwork_t() : userIdentity(false), userDestination({RNS::Type::NONE}) {};
    const unsigned long networkInterval = 50;

public:
    unsigned long loop();

    // members
private:
    RNS::Identity userIdentity;
    RNS::Destination userDestination;
    // functions
private:
    void onPacket(const RNS::Bytes &data, const RNS::Packet &packet);
    void reticulum_announce();
    void send_packet();
    // announce handler
private:
    class announceHandlerClass : public RNS::AnnounceHandler
    {
    public:
        announceHandlerClass(const char *aspect_filter = nullptr) : AnnounceHandler(aspect_filter) {}
        ~announceHandlerClass() {}
        void received_announce(const RNS::Bytes &destination_hash, const RNS::Identity &announced_identity, const RNS::Bytes &app_data)
        {
            Serial.println("Setting external destination.....");
            // externDestination = RNS::Destination(announced_identity, RNS::Type::Destination::IN, RNS::Type::Destination::SINGLE, destination_hash);
            Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            Serial.printf("announceHandlerClass: destination hash: %d\n", destination_hash.toHex());
            if (announced_identity)
            {
                Serial.printf("announceHandlerClass: announced identity hash: %d\n", announced_identity.hash().toHex());
                Serial.printf("announceHandlerClass: announced identity app data: %d\n", announced_identity.app_data().toHex());
            }
            if (app_data)
            {
                Serial.printf("announceHandlerClass: app data text: %s\n", app_data.toString());
            }
            Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        }
    };
};

extern managerNetwork_t &managerNetwork;
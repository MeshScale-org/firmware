#pragma once
#include "manager_base.h"
#include "managerSystem.h"
#include "Reticulum.h"

/*
The network Manager

Jobs:


    ...
*/

class managerNetwork_t : public manager_base
{
    // singleton class
public:
    managerNetwork_t(managerNetwork_t &) = delete;
    // get singleton instance
    inline static managerNetwork_t &getInstance()
    {
        static managerNetwork_t instance;
        return instance;
    };

private:
    managerNetwork_t() {};
    const unsigned long networkInterval = 50;

public:
    unsigned long loop();

    // members
private:
    RNS::Identity *userIdentity;
    RNS::Destination *userDestination;

    // functions
private:
    void onPacket(const RNS::Bytes &data, const RNS::Packet &packet);
    void reticulum_announce();
    void send_packet();

    // Handle specific pbMessage types
    void handleGetReq(meshScale_Instruction_GetReq &getReq, meshScale_Source &source) override {};
    void handleGetResp(meshScale_Instruction_GetResp &getResp, meshScale_Source &source) override;
    void handleSetReq(meshScale_Instruction_SetReq &setReq, meshScale_Source &source) override {};
    void handleSetResp(meshScale_Instruction_SetResp &setResp, meshScale_Source &source) override {};
    void handleExecReq(meshScale_Instruction_ExecReq &execReq, meshScale_Source &source) override;
    void handleExecResp(meshScale_Instruction_ExecResp &execResp, meshScale_Source &source) override {};

private:
    class announceHandlerClass : public RNS::AnnounceHandler
    {
    public:
        announceHandlerClass(const char *aspect_filter = nullptr) : AnnounceHandler(aspect_filter) {}
        ~announceHandlerClass() {}
        void received_announce(const RNS::Bytes &destination_hash, const RNS::Identity &announced_identity, const RNS::Bytes &app_data)
        {
            if (destination_hash && announced_identity)
            {
                Serial.println("managerNetwork.h::announceHandler: Succesfully received announce ...");
                std::shared_ptr<meshScale_PbMessage> newMessage = std::make_shared<meshScale_PbMessage>();
                *newMessage = meshScale_PbMessage_init_zero;
                newMessage->which_content = meshScale_PbMessage_event_tag;
                newMessage->event.has_event_source = true;
                newMessage->event.event_source.which_source = meshScale_Source_local_source_tag;
                newMessage->event.event_source.local_source = meshScale_Source_LocalSource_MANAGER_NETWORK;
                newMessage->event.which_proto_module = meshScale_Event_announce_events_tag;
                newMessage->event.announce_events.which_announce_event = meshScale_Announce_AnnounceEvents_incoming_announce_tag;
                newMessage->event.announce_events.incoming_announce.has_destination_hash = true;
                memcpy(newMessage->event.announce_events.incoming_announce.destination_hash.hash, destination_hash.data(), 16);              // TODO: no magic numbers
                memcpy(newMessage->event.announce_events.incoming_announce.public_keys.key, announced_identity.get_public_key().data(), 64); // TODO: no magic numbers
                // Send the event to managerSystem
                managerSystem.takePbMessage(newMessage);
            }
            else
            {
                Serial.println("managerNetwork.h::announceHandler: Received announce with error...");
            }
        }
    };
};

extern managerNetwork_t &managerNetwork;
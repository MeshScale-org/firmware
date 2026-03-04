#include "managerNetwork.h"
#include "main.h"

// We initialise two lists of strings to use as app_data
const char *noble_gases[] = {"Helium", "Neon", "Argon", "Krypton", "Xenon", "Radon", "Oganesson"};
// init myDestination
RNS::Destination myDestination({RNS::Type::NONE});

//  Test AnnounceHandler
class ExampleAnnounceHandler : public RNS::AnnounceHandler
{
public:
    ExampleAnnounceHandler(const char *aspect_filter = nullptr) : AnnounceHandler(aspect_filter) {}
    virtual ~ExampleAnnounceHandler() {}
    virtual void received_announce(const RNS::Bytes &destination_hash, const RNS::Identity &announced_identity, const RNS::Bytes &app_data)
    {
        Serial.println("Setting external destination.....");
        externDestination = RNS::Destination(announced_identity, RNS::Type::Destination::IN, RNS::Type::Destination::SINGLE, destination_hash);
        Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        Serial.printf("ExampleAnnounceHandler: destination hash: %d\n", destination_hash.toHex());
        if (announced_identity)
        {
            Serial.printf("ExampleAnnounceHandler: announced identity hash: %d\n", announced_identity.hash().toHex());
            Serial.printf("ExampleAnnounceHandler: announced identity app data: %d\n", announced_identity.app_data().toHex());
        }
        if (app_data)
        {
            Serial.printf("ExampleAnnounceHandler: app data text: %s\n", app_data.toString());
        }
        Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
};

//  Test packet receive callback
void onPacket(const RNS::Bytes &data, const RNS::Packet &packet)
{
    Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    Serial.printf("onPacket: data: %d\n", data.toHex());
    Serial.printf("onPacket: text: %s\n", data.toString());
    TRACE("onPacket: " + packet.debugString());
    Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    RNS::Packet newPack(packet);
    newPack.unpack();

    TRACE("Test recv_packet: " + newPack.debugString());
}

RNS::HAnnounceHandler announce_handler(new ExampleAnnounceHandler());

void reticulum_announce()
{
    if (myDestination)
    {
        Serial.printf("Announcing destination...\n");
        myDestination.announce(RNS::bytesFromString(noble_gases[RNS::Cryptography::randomnum() % 7]));
    }
    else
    {
        Serial.printf("Cant announce, no destination\n");
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
        switch (incoming.which_content)
        {
        case meshScale_PbMessage_chat_action_tag:
            if (incoming.content.chat_action.which_action == meshScale_SendChat_chat_tag)
            {
                Serial.printf("managerNetwork_t: received pbMessage to send %s to destination %d\n", incoming.content.chat_action.action.send_chat.chat.text_string, incoming.content.chat_action.action.send_chat.destination_hash);
            }
            break;

        case meshScale_PbMessage_announce_action_tag:
            if (incoming.content.announce_action.which_action == meshScale_AnnounceAction_single_announce_tag)
            {
                Serial.printf("managerNetwork_t: received pbMessage to announce my destination\n");
                reticulum_announce();
            }
            break;

        default:
            Serial.println("[managerNetwork_t::loop] unknown pbMessage content type, removing");
            break;
        }

        pbMessagesIn.pop();
    }

    // TODO: reticulum loop should not call the interface hardware, only add commands to a queue
    std::lock_guard<resourceLock> lg(handlerNetworkInterfaces);
    handlerNetworkInterfaces.loop();
    reticulum.loop();

    return networkInterval;

    /*
        Serial.println("Creating Destination instance...");
        destination = RNS::Destination(*managerSystem.getInstance().getIdentity(), RNS::Type::Destination::IN, RNS::Type::Destination::SINGLE, "meshScale", "PbMessage");

        // Register DATA packet callback
        Serial.println("Registering packet callback with Destination...");
        destination.set_packet_callback(onPacket);
        destination.set_proof_strategy(RNS::Type::Destination::PROVE_ALL);

        Serial.println("Registering announce handler with Transport...");
        RNS::Transport::register_announce_handler(announce_handler);
        initDone = true;
        return 50;
    */
};

managerNetwork_t &managerNetwork = managerNetwork_t::getInstance();

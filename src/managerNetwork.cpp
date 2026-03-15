#include "managerNetwork.h"
#include "main.h"

// We initialise two lists of strings to use as app_data
const char *noble_gases[] = {"Helium", "Neon", "Argon", "Krypton", "Xenon", "Radon", "Oganesson"};
// init userDestination
// RNS::Destination userDestination({RNS::Type::NONE});

//  Test packet receive callback
void managerNetwork_t::onPacket(const RNS::Bytes &data, const RNS::Packet &packet)
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

// RNS::HAnnounceHandler announce_handler(new managerNetwork_t::announceHandlerClass());

void managerNetwork_t::reticulum_announce()
{
    if (userDestination)
    {
        Serial.printf("Announcing destination...\n");
        userDestination.announce(RNS::bytesFromString(noble_gases[RNS::Cryptography::randomnum() % 7]));
    }
    else
    {
        Serial.printf("Cant announce, no destination\n");
    }
}

void managerNetwork_t::send_packet()
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
    handlePbMessages();
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

void managerNetwork_t::handleExecReq(meshScale_Instruction_ExecReq &execReq, meshScale_Source &source)
{
    switch (execReq.command.which_proto_module)
    {
    case meshScale_Instruction_Command_chat_commands_tag:
        if (execReq.command.chat_commands.which_command == meshScale_Chat_ChatCommands_send_chat_tag && execReq.command.chat_commands.send_chat.has_destination_hash)
        {
            Serial.printf("managerNetwork_t: received send chat command to send %s to destination %s \n",
                          execReq.command.chat_commands.send_chat.text_string, execReq.command.chat_commands.send_chat.destination_hash.hash);
        }
        else
        {
            Serial.printf("managerNetwork_t: error handling send chat command \n");
        }
        break;

    case meshScale_Instruction_Command_announce_commands_tag:
        if (execReq.command.announce_commands.which_command == meshScale_Announce_AnnounceCommands_do_announce_command_tag)
        {
            Serial.printf("managerNetwork_t: received command to announce my destination\n");
            reticulum_announce();
        }
        break;

    // unknown instruction type
    default:
        Serial.printf("[managerNetwork_t::loop] unhandled Instruction.execReq type: %d\n", execReq.command.which_proto_module);
        break;
    }
};

managerNetwork_t &managerNetwork = managerNetwork_t::getInstance();

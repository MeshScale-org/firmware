#include "managerNetwork.h"
#include "main.h"

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

/*
 * Network operations
 */
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

void managerNetwork_t::reticulum_announce()
{
    if (*userDestination)
    {
        Serial.printf("Announcing destination...\n");
        userDestination->announce(/*RNS::bytesFromString(noble_gases[RNS::Cryptography::randomnum() % 7])*/);
    }
    else
    {
        Serial.printf("Cant announce, no destination\n");
        // request the identity private key from managerSystem so we can create userIdent and userDest
        std::shared_ptr<meshScale_PbMessage> newMessage = std::make_shared<meshScale_PbMessage>();
        *newMessage = meshScale_PbMessage_init_zero;
        newMessage->which_content = meshScale_PbMessage_instruction_tag;
        newMessage->instruction.has_requester = true;
        newMessage->instruction.requester.which_source = meshScale_Source_local_source_tag;
        newMessage->instruction.requester.local_source = meshScale_Source_LocalSource_MANAGER_NETWORK;
        newMessage->instruction.which_instruction = meshScale_Instruction_get_req_tag;
        newMessage->instruction.get_req.has_attribute = true;
        newMessage->instruction.get_req.attribute.which_proto_module = meshScale_Instruction_Attribute_identity_attributes_tag;
        newMessage->instruction.get_req.attribute.identity_attributes = meshScale_Identity_IdentityAttributes_IDENTITY_CONFIG;
        // Send the event to managerSystem
        managerSystem.takePbMessage(newMessage);
    }
}

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

/*
 * Handle incoming pbMessages
 */
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
            Serial.printf("managerNetwork_t: error handling chat command \n");
        }
        break;
    case meshScale_Instruction_Command_announce_commands_tag:
        if (execReq.command.announce_commands.which_command == meshScale_Announce_AnnounceCommands_do_announce_command_tag)
            reticulum_announce();
        break;
    // unknown instruction type
    default:
        Serial.printf("[managerNetwork_t::loop] unhandled Instruction.execReq type: %d\n", execReq.command.which_proto_module);
        break;
    }
};

void managerNetwork_t::handleGetResp(meshScale_Instruction_GetResp &getResp, meshScale_Source &source)
{
    // only do something when the response status is succes
    if (getResp.response_status == meshScale_Instruction_ResponseStatus_SUCCES)
    {
        switch (getResp.value.which_proto_module)
        {
        case meshScale_Instruction_Value_identity_values_tag:
            // received identity config, update the userIdentity if the config contains the private keys
            if (getResp.value.identity_values.which_value == meshScale_Identity_IdentityValues_identity_config_tag && getResp.value.identity_values.identity_config.has_private_keys)
            {
                // if there already is a userdestination, delete it first
                if (*userIdentity)
                    delete userIdentity;
                userIdentity = new RNS::Identity(false); // dont create keys

                // try loading the private key
                if (userIdentity->load_private_key(RNS::bytesFromChunk(&getResp.value.identity_values.identity_config.private_keys.key[0], 64)))
                {
                    // if there already is a userdestination, delete it first
                    if (*userDestination)
                        delete userDestination;
                    userDestination = new RNS::Destination(userIdentity, RNS::Type::Destination::IN, RNS::Type::Destination::SINGLE, "app", "aspects");

                    Serial.printf("Manager network: Updated private keys of ident and dest \n");
                }
            }
            break;

        default:
            break;
        }
    }
};

managerNetwork_t &managerNetwork = managerNetwork_t::getInstance();

#include "managerSystem.h"
#include "Reticulum.h"

bool initDone = false;
unsigned long managerSystem_t::loop()
{
        if (initDone)
        {
                return 0; // no jobs
        }
        else
        {
                RNS::Identity myIdentity = RNS::Identity(false);
                RNS::Bytes prv_bytes;

// static keys for testing, TODO: change
// should be created/loaded from memory(/received from client)
#ifdef ESP32
                prv_bytes.assignHex("78E7D93E28D55871608FF13329A226CABC3903A357388A035B360162FF6321570B092E0583772AB80BC425F99791DF5CA2CA0A985FF0415DAB419BBC64DDFAE8");
#else
                prv_bytes.assignHex("E0D43398EDC974EBA9F4A83463691A08F4D306D4E56BA6B275B8690A2FBD9852E9EBE7C03BC45CAEC9EF8E78C830037210BFB9986F6CA2DEE2B5C28D7B4DE6B0");
#endif

                myIdentity.load_private_key(prv_bytes);

                // create our destination
                RNS::Destination myDestination = RNS::Destination(myIdentity, RNS::Type::Destination::IN, RNS::Type::Destination::SINGLE, "meshScale", "PbMessage");

                // we have a new myDestiantion, so send it to networkManager so it can be announced
                // send here

                initDone = true;
                return systemInterval; // time until next run
        }
}

managerSystem_t &managerSystem = managerSystem_t::getInstance();


#pragma once
#include <Reticulum.h>

class handlerNetworkInterfaces_t;

class managedInterfaceImpl_t : public RNS::InterfaceImpl
{
public:
    // types
#pragma region ifTypesDefinitions

    // possible interface types
    enum ifType_t
    {
        IF_NONE,
        IF_RADIOLIB,
        IF_UDP
    };

    // radiolib: Radio type
    enum radiolibRadioTypes_t
    {
        RADIO_NONE,
        RADIO_SX1262
    };
    // radiolib: Modems
    enum radiolibModems_t
    {
        MODEM_NONE,
        MODEM_LORA,
        MODEM_FSK
    };

    // radiolib modem: Lora
    struct loraConfig_t
    {
        float frequency = 0;
        float bandwidth = 0;
        uint8_t spreadingFactor = 0;
        uint8_t codingRate = 0;
        uint8_t syncWord = 0;
        int8_t power = 0; // in dbm
        uint16_t preambleLength = 0;
    };
    // radiolib modem: FSK
    struct fskConfig_t
    {
        float frequency = 0;
        float bitRate = 0;
        float frequencyDeviation = 0;
        float rxBandwidth = 0;
        int8_t power = 0;
        uint16_t preambleLength = 0;
    };

    // all radiolib modem configs
    union modemConfig_t
    {
        loraConfig_t loraConfig;
        fskConfig_t fskConfig;
    };

    // radiolib all
    struct radiolibConfig_t
    {
        radiolibRadioTypes_t radioType = RADIO_NONE;
        radiolibModems_t modemType = MODEM_NONE;
        modemConfig_t modemConfig = {}; // init something (all 0)
    };

    // UDP: Medium
    enum udpMediums_t
    {
        MEDIUM_NONE,
        MEDIUM_ETH,
        MEDIUM_WIFI
    };

    // UDP: Network
    struct udpNetwork_t
    {
        const char *SSID;
        const char *KEY;
        uint16_t port = 0;     // TODO
        uint16_t ipAdress = 0; // TODO
    };

    // UDP all
    struct udpConfig_t
    {
        udpMediums_t medium = MEDIUM_NONE;
        udpNetwork_t network;
    };

    // all
    union interfaceConfig_t
    {
        radiolibConfig_t radiolibConfig;
        udpConfig_t udpConfig;
    };

#pragma endregion ifTypesDefinitions

    struct managedInterfaceConfig_t
    {
        ifType_t ifType = IF_NONE;
        interfaceConfig_t interfaceConfig = {}; // init something
        RNS::Type::Interface::modes rnsIfMode = RNS::Type::Interface::modes::MODE_NONE;
    };

public:
    managedInterfaceImpl_t(std::string name) : RNS::InterfaceImpl(name.c_str()) {};

    virtual bool updateConfig(managedInterfaceImpl_t::managedInterfaceConfig_t rnsInterfaceDescription) = 0;
};
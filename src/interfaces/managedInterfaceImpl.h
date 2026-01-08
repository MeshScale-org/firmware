
#pragma once
#include <Reticulum.h>

class interfaceManager;

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
        CONFIG_NONE,
        CONFIG_LORA
    };

    // radiolib modem: Lora
    struct loraConfig_t
    {
        float frequency;
        float bandwidth;
        uint8_t spreadingFactor;
        uint8_t codingRate;
        uint8_t syncWord;
        uint8_t power;
        uint16_t preambleLength;
    };

    // all radiolib modem configs
    union radiolibModemConfig_t
    {
        loraConfig_t loraConfig;
    };

    // radiolib all
    struct radiolibConfig_t
    {
        radiolibRadioTypes_t radioType;
        radiolibModems_t configType;
        radiolibModemConfig_t radiolibModemConfig;
    };

    // UDP: Medium
    enum udpMediums_t
    {
        NONE,
        ETH,
        WIFI
    };

    // UDP: Network
    struct udpNetwork_t
    {
        const char *SSID;
        const char *KEY;
        uint16_t port;     // TODO
        uint16_t ipAdress; // TODO
    };

    // UDP all
    struct udpConfig_t
    {
        udpMediums_t medium;
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
        interfaceConfig_t interfaceConfig;

        RNS::Type::Interface::modes rnsIfMode = RNS::Type::Interface::modes::MODE_NONE;
    };

public:
    managedInterfaceImpl_t(const char *name) : RNS::InterfaceImpl(name) {};

    // virtual bool configManagedIf(rnsInterfaceDescription_t rnsInterfaceDescription) const = 0;
};
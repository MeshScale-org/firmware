
#include <Arduino.h>
#include <vector>
#include <Reticulum.h>
#include <RadioLib.h>

// add interface classes
#include "interfaces/radioLibInterface.h"
#include "interfaces/UDPInterface.h"

class interfaceManager
{
public:
// types
#if true // collapse types
    // enum of possible interface types
    enum ifDescriptionType_t
    {
        IF_NONE,
        IF_SX1262,
        IF_UDP_WIFI,
        IF_UPD_ETH,
        IF_UART
    };

    // possible hardware descriptions of an interface (includes capabilities)
    struct radiolibDescription_t
    {
        PhysicalLayer *radio;
        float tcxoVoltage;
        bool useRegulatorLDO;
    };

    struct wifiDescription_t
    {
    };

    union ifDescription_t
    {
        radiolibDescription_t radiolibDescription;
        wifiDescription_t wifiDescription;
    };

    // enum of possible config types of an interface
    enum ifConfigTypes_t
    {
        CONFIG_NONE,
        CONFIG_LORA,
        CONFIG_FSK,
        CONFIG_UDP_WIFI,
        CONFIG_UDP_ETH,
        CONFIG_UART
    };

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

    struct wifiConfig_t
    {
        const char *SSID;
        const char *KEY;
    };

    union ifConfig_t
    {
        loraConfig_t loraConfig;
        wifiConfig_t wifiConfig;
    };

    // managed interface data with its type, hardware description/capabilities and config
    // Would use std::variant (c++17) instead of unions but gives all sorts of problems because its not as supported
    struct managedIf_t
    {
        String name = "";
        ifDescriptionType_t ifDescriptionType = IF_NONE;
        ifDescription_t ifDescription;
        ifConfigTypes_t ifConfigType = CONFIG_NONE;
        ifConfig_t ifConfig;
        RNS::Type::Interface::modes rnsIfMode = RNS::Type::Interface::modes::MODE_NONE;
    };
#endif // true

public:
    interfaceManager() {};
    interfaceManager(interfaceManager &) = delete;
    static bool addInterface(managedIf_t newInterface) { return get().addInterfaceImpl(&newInterface); }
    static String interfacesToString(bool verbose = false) { return get().interfacesToStringImpl(verbose); };

private:
    // get singleton instance
    static interfaceManager &get()
    {
        static interfaceManager instance;
        return instance;
    }
    bool addInterfaceImpl(managedIf_t *newInterface);
    String interfacesToStringImpl(bool verbose);

private:
    bool checkNewConfig(managedIf_t *newInterface);

    std::vector<managedIf_t> interfaces;
};

#include "handlerNetworkInterfaces.h"

#include "networkInterfaces/radiolibInterfaceAdapters/radiolibInterfaceAdapters_includeAll.h"
#include "networkInterfaces/radioLibInterface.h"
#include "networkInterfaces/UDPInterface.h"

bool handlerNetworkInterfaces_t::addInterface(uint8_t ifID, managedInterface_t *newInterface, bool autoStart /*=true*/)
{
    if (newInterface)
    {

        // TODO check if inset was succes, if not delete newInterface and return false
        interfaces.insert({ifID, newInterface});
        // start radio with initial config

        newInterface->managedInterfaceImpl->updateConfig(newInterface->managedInterfaceConfig);
        if (autoStart)
        {
            // TODO: check if start is succes, if not delete newInterface and return false
            newInterface->transportIf.start();
        }
        return true;
    }
    else
    {
        Serial.println("adding interface not possible: nullptr");
        return false;
    }
};

// factory for radiolib interface
handlerNetworkInterfaces_t::managedInterface_t *handlerNetworkInterfaces_t::createInterface(std::string ifName, managedInterfaceImpl_t::managedInterfaceConfig_t ifConfig, radioLimits_t radioLimits, uint32_t cs, uint32_t irq, uint32_t rst, uint32_t gpio, SPIClassL &spi, SPISettings spiSettings)
{
    Serial.printf("Creating interface %s: ", ifName.c_str());
    radiolibInterfaceAdapter_base *radioAdapter = nullptr;
    managedInterface_t *managedInterface = new managedInterface_t;

    if (ifConfig.ifType == managedInterfaceImpl_t::IF_RADIOLIB)
    {
        Serial.printf("IF_RADIOLIB, ");
        Module *ifModule = new Module(cs, irq, rst, gpio, spi.get(), spiSettings);

        switch (ifConfig.interfaceConfig.radiolibConfig.radioType)
        {
        case managedInterfaceImpl_t::RADIO_SX1262:
            Serial.printf("RADIO_SX1262, ");
            radioAdapter = new SX1262Adapter(new SX1262(ifModule), radioLimits, spi);
            break;
        default: // radio not supported
            delete ifModule;
            return nullptr;
            break;
        }

        // populate managedInterface
        managedInterface->managedInterfaceConfig = ifConfig;
        managedInterface->managedInterfaceImpl = new radioLibInterface(ifName, irq, spi, radioAdapter);
        managedInterface->transportIf = RNS::Interface(managedInterface->managedInterfaceImpl);
        Serial.printf("Creation succes\n");
        return managedInterface;
    }
    else
    {
        delete managedInterface;
        Serial.println("Wrong createInterface overload called");
        return nullptr;
    }
};

#ifndef EXCLUDE_INTERFACE_UDP
// factory for UDP interface
handlerNetworkInterfaces_t::managedInterface_t *handlerNetworkInterfaces_t::createInterface(std::string ifName, managedInterfaceImpl_t::managedInterfaceConfig_t ifConfig)
{
    if (ifConfig.ifType == managedInterfaceImpl_t::IF_UDP)
    {
        managedInterface_t *managedInterface = new managedInterface_t;
        managedInterface->managedInterfaceConfig = ifConfig;
        managedInterface->managedInterfaceImpl = new UDPInterface(ifName);
        managedInterface->transportIf = RNS::Interface(managedInterface->managedInterfaceImpl);
        return managedInterface;
    }
    else
    {
        Serial.println("Wrong createInterface overload called");
        return nullptr;
    }
};
#endif

bool handlerNetworkInterfaces_t::configureInterface(uint8_t ifID, managedInterfaceImpl_t::managedInterfaceConfig_t newConfig)
{
    if (interfaces.find(ifID) != interfaces.end())
    {
        interfaces[ifID]->transportIf.stop();
        if (interfaces[ifID]->managedInterfaceImpl->updateConfig(newConfig))
        {
            Serial.printf("updating interface %s succes, Starting Interface\n", interfaces[ifID]->transportIf.name().c_str());
            if (interfaces[ifID]->transportIf.start())
            {
                Serial.printf("Interface started\n");
                interfaces[ifID]->managedInterfaceConfig = newConfig;
                return true;
            }
            else
            {
                Serial.printf("Interface failed to start\n");
                return false;
            }
        }
        else
        {
            Serial.printf("updating interface %s failed, interface Stopped\n", interfaces[ifID]->transportIf.name().c_str());
            return false;
        }
    }
    else
    {
        Serial.printf("handlerNetworkInterfaces_t::configureInterfaceImpl: ERROR: Interface with id: %d does not exist, interface config not changed\n", ifID);
        return false;
    }
};

bool handlerNetworkInterfaces_t::registerIfsTransport()
{
    bool ret = false;
    for (auto it = interfaces.begin(); it != interfaces.end(); ++it)
    {
        if (RNS::Transport::find_interface_from_hash(it->second->transportIf.get_hash()).get() == nullptr)
        {
            ret = true;
            Serial.printf("Registering interface %s to transport\n", it->second->transportIf.name().c_str());
            RNS::Transport::register_interface(it->second->transportIf);
        }
    }

    return ret;
};

void handlerNetworkInterfaces_t::loop()
{
    for (auto it = interfaces.begin(); it != interfaces.end(); ++it)
    {
        if (it->second && it->second->managedInterfaceImpl)
        {
            it->second->transportIf.loop();
            return;
        }
        Serial.printf("handlerNetworkInterfaces_t::loopImpl: ERROR: nullptr error\n");
    }
};

String handlerNetworkInterfaces_t::interfacesToString(bool verbose)
{
    String outString = "";
    outString += "Number of Interfaces: ";
    outString += interfaces.size();
    outString += "\n\n";

    for (auto it = interfaces.begin(); it != interfaces.end(); ++it)
    {
        outString += "ifID " + String(it->first);
        outString += ":\n";
        outString += "Name: ";
        if (it->second->transportIf) // Create transportIf object first
        {
            outString += it->second->transportIf.name().c_str();
        }
        else
        {
            outString += "#####";
        }
        outString += ", Type: ";

        switch (it->second->managedInterfaceConfig.ifType)
        {
        case managedInterfaceImpl_t::IF_RADIOLIB:
            outString += ("IF_RADIOLIB\n");
            break;
        case managedInterfaceImpl_t::IF_UDP:
            outString += ("IF_UDP\n");
            break;
        default:
            outString += ("undefined\n");
            break;
        }

        if (verbose)
        {
            switch (it->second->managedInterfaceConfig.ifType)
            {
            case managedInterfaceImpl_t::IF_RADIOLIB:

                switch (it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemType)
                {
                case managedInterfaceImpl_t::MODEM_LORA:
                    outString += "Modem: Lora\n";
                    outString += "\tFrequency: " + String(it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.frequency, 3) + "\n";
                    outString += "\tBandwidth: " + String(it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.bandwidth, 3) + "\n";
                    outString += "\tSpreadingFactor: " + String(it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.spreadingFactor, 3) + "\n";
                    outString += "\tCodingRate: " + String(it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.codingRate, 3) + "\n";
                    outString += "\tSyncWord: " + String(it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.syncWord, 3) + "\n";
                    outString += "\tPower: " + String(it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.power, 3) + "\n";
                    outString += "\tPreambleLength: " + String(it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.preambleLength, 3) + "\n";
                    break;
                case managedInterfaceImpl_t::MODEM_FSK:
                    outString += "Modem: FSK\n";
                    outString += "\tFrequency: " + String(it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.fskConfig.frequency, 3) + "\n";
                    outString += "\tBitRate: " + String(it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.fskConfig.bitRate, 3) + "\n";
                    outString += "\tFrequencyDeviation: " + String(it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.fskConfig.frequencyDeviation, 3) + "\n";
                    outString += "\tRxBandwidth: " + String(it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.fskConfig.rxBandwidth, 3) + "\n";
                    outString += "\tPower: " + String(it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.fskConfig.power, 3) + "\n";
                    outString += "\tPreambleLength: " + String(it->second->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.fskConfig.preambleLength, 3) + "\n";
                    break;
                default:
                    break;
                }
                break;
            case managedInterfaceImpl_t::IF_UDP:
                outString += "SSID: " + String(it->second->managedInterfaceConfig.interfaceConfig.udpConfig.network.SSID) + "\n";
                break;
            default:
                outString += ("undefined\n");
                break;
            }
        }
        outString += "\n";
    }

    return outString;
};

handlerNetworkInterfaces_t &handlerNetworkInterfaces = handlerNetworkInterfaces_t::getInstance();
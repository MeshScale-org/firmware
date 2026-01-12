#include "interfaceManager.h"

bool interfaceManager::addInterfaceImpl(uint8_t ifID, managedInterface_t *newInterface, bool autoStart /*=true*/)
{
    newInterface->transportIf = RNS::Interface(newInterface->managedInterfaceImpl);
    // interfaces.push_back(newInterface);
    interfaces.insert({ifID, newInterface});
    // start radio with initial config
    newInterface->managedInterfaceImpl->updateConfig(newInterface->managedInterfaceConfig);
    if (autoStart)
    {
        newInterface->transportIf.start();
    }
    return true;
};

bool interfaceManager::configureInterfaceImpl(uint8_t ifID, managedInterfaceImpl_t::managedInterfaceConfig_t newConfig)
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
        Serial.printf("interfaceManager::configureInterfaceImpl: ERROR: Interface with id: %d does not exist, interface config not changed\n", ifID);
        return false;
    }
};

bool interfaceManager::registerIfsTransportImpl()
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

void interfaceManager::loopImpl()
{
    for (auto it = interfaces.begin(); it != interfaces.end(); ++it)
    {
        if (it->second && it->second->managedInterfaceImpl)
        {
            it->second->transportIf.loop();
            return;
        }
        Serial.printf("interfaceManager::loopImpl: ERROR: nullptr error\n");
    }
};

String interfaceManager::interfacesToStringImpl(bool verbose)
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
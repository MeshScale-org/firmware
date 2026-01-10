#include "interfaceManager.h"

bool interfaceManager::addInterfaceImpl(managedInterface_t *newInterface)
{
    newInterface->RNS_IF = RNS::Interface(newInterface->managedInterfaceImpl);
    interfaces.push_back(newInterface);
    // start radio with initial config
    // newInterface->managedInterfaceImpl->updateConfig(newInterface->managedInterfaceConfig);
    return true;
};

bool interfaceManager::configureInterfaceImpl(uint8_t interfaceIndex, managedInterfaceImpl_t::managedInterfaceConfig_t newConfig)
{
    interfaces[interfaceIndex]->RNS_IF.stop();
    if (interfaces[interfaceIndex]->managedInterfaceImpl->updateConfig(newConfig))
    {
        Serial.printf("updating interface %s succes, Starting Interface\n", interfaces[interfaceIndex]->RNS_IF.name().c_str());
        if (interfaces[interfaceIndex]->RNS_IF.start())
        {
            Serial.printf("Interface started\n");
            interfaces[interfaceIndex]->managedInterfaceConfig = newConfig;
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
        Serial.printf("updating interface %s failed, interface Stopped\n", interfaces[interfaceIndex]->RNS_IF.name().c_str());
        return false;
    }
};

bool interfaceManager::registerIfsTransportImpl()
{
    bool ret = false;
    for (uint8_t i = 0; i < interfaces.size(); i++)
    {
        // std::map<Bytes, Interface&>&registeredInterfaces
        auto &registeredInterfaces = RNS::Transport::get_interfaces();
        if (RNS::Transport::find_interface_from_hash(interfaces[i]->RNS_IF.get_hash()).get() == nullptr)
        {
            ret = true;
            Serial.printf("Registering interface %s to transport\n", interfaces[i]->RNS_IF.name().c_str());
            RNS::Transport::register_interface(interfaces[i]->RNS_IF);
        }
    }

    return ret;
};

void interfaceManager::loopImpl()
{
    for (uint8_t i = 0; i < interfaces.size(); i++)
    {
        if (interfaces[i] && interfaces[i]->managedInterfaceImpl)
        {
            interfaces[i]->RNS_IF.loop();
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
    for (uint8_t i = 0; i < interfaces.size(); i++)
    {
        // always display name and ifDescriptionType
        outString += i;
        outString += ":\n";
        outString += "Name: ";
        if (interfaces[i]->RNS_IF) // Create RNS_IF object first
        {
            outString += interfaces[i]->RNS_IF.name().c_str();
        }
        else
        {
            outString += "#####";
        }
        outString += ", ";
        switch (interfaces[i]->managedInterfaceConfig.ifType)
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

        // TODO: add verbose mode that shows freq,bw,sf,... in case of loraconfig. SSID, .. in case of wifi, ...
        if (verbose)
        {
            switch (interfaces[i]->managedInterfaceConfig.ifType)
            {
            case managedInterfaceImpl_t::IF_RADIOLIB:

                switch (interfaces[i]->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemType)
                {
                case managedInterfaceImpl_t::MODEM_LORA:
                    outString += "Modem: Lora\n";
                    outString += "\tFrequency: " + String(interfaces[i]->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.frequency, 3) + "\n";
                    break;
                case managedInterfaceImpl_t::MODEM_FSK:
                    outString += "Modem: FSK\n";
                    outString += "\tFrequency: " + String(interfaces[i]->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.fskConfig.frequency, 3) + "\n";
                    break;
                default:
                    break;
                }
                break;
            case managedInterfaceImpl_t::IF_UDP:
                outString += "SSID: " + String(interfaces[i]->managedInterfaceConfig.interfaceConfig.udpConfig.network.SSID) + "\n";
                break;
            default:
                outString += ("undefined\n");
                break;
            }
        }
    };

    return outString;
};
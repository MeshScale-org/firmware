#include "interfaceManager.h"

bool interfaceManager::addInterfaceImpl(managedInterface_t *newInterface)
{
    newInterface->RNS_IF = RNS::Interface(newInterface->managedInterfaceImpl);
    interfaces.push_back(newInterface);
    return true;
};

bool interfaceManager::updateTransportInterfacesImpl()
{
    for (uint8_t i = 0; i < interfaces.size(); i++)
    {
        if (interfaces[i]->update)
        {
            /*
            // is deregister/register really needed?

            // interface stop
            interfaces[i]->RNS_IF.stop();
            if (interfaces[i]->managedInterfaceImpl->updateConfig(interfaces[i]->managedInterfaceConfig))
            {
                Serial.printf("updating interface %s succes, Starting Interface\n", interfaces[i]->RNS_IF.name().c_str());
                if (interfaces[i]->RNS_IF.start())
                {
                    Serial.printf("Interface started\n");
                }
                else
                {
                    Serial.printf("Interface failed to start\n");
                }
            }
            else
            {
                Serial.printf("updating interface %s failed, Starting Stopped\n", interfaces[i]->RNS_IF.name().c_str());
            }*/

            interfaces[i]->update = false;
        }
    }

    return true;
};

String interfaceManager::interfacesToStringImpl(bool verbose)
{
    String outString = "";
    outString += "Number of Interfaces: ";
    outString += interfaces.size();
    outString += "\n\n";
    for (uint16_t i = 0; i < interfaces.size(); i++)
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
        }
    };

    return outString;
};
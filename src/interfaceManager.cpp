#include "interfaceManager.h"

bool interfaceManager::checkNewConfig(managedIf_t *newInterface)
{
    return true;
};

bool interfaceManager::addInterfaceImpl(managedIf_t *newInterface)
{
    // new interface so it should be updated in rns transport
    newInterface->update = true;
    if (checkNewConfig(newInterface))
    {
        interfaces.push_back(newInterface);
        return true;
    }
    else
    {
        return false;
    }
};

bool interfaceManager::updateTransportInterfacesImpl()
{

    for (uint8_t i = 0; i < interfaces.size(); i++)
    {
        if (interfaces[i]->update)
        {

            // is deregister/register really needed?

            // impl stop
            // rns deregister
            // impl change config
            // impl start
            // rns register

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
        outString += interfaces[i]->name;
        outString += ", ";
        switch (interfaces[i]->ifDescriptionType)
        {
        case IF_RADIOLIB:
            outString += ("IF_RADIOLIB\n");
            break;
        case IF_UDP_WIFI:
            outString += ("IF_UDP_WIFI\n");
            break;
        default:
            outString += ("undefined\n");
            break;
        }

        // TODO: add verbose mode that shows freq,bw,sf,... in case of loraconfig. SSID, .. in case of wifi, ...
        if (verbose)
        {
            outString += ("verbose not implemented\n");
            switch (interfaces[i]->ifConfigType)
            {
            case CONFIG_LORA:

                break;
            case CONFIG_UDP_WIFI:

            default:
                outString += ("Error: undefined ifConfigType\n");
                break;
            }
        }
    };

    return outString;
};
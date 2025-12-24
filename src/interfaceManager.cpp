#include "interfaceManager.h"

bool interfaceManager::checkNewConfig(managedIf_t *newInterface)
{
    return true;
};

bool interfaceManager::addInterfaceImpl(managedIf_t *newInterface)
{
    if (checkNewConfig(newInterface))
    {
        interfaces.push_back(*newInterface);
        return true;
    }
    else
    {
        return false;
    }
};

String interfaceManager::interfacesToStringImpl()
{
    String outString = "";
    Serial.printf("Interfaces size: %d\n", interfaces.size());
    for (uint16_t i = 0; i < interfaces.size(); i++)
    {
        outString += interfaces[i].toString();
        outString += ",\n";
    };
    return outString;
};
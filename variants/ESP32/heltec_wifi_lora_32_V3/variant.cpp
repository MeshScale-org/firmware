#include "variant.h"
#include "interfaceManager.h"
#include "../../../credentials.h"

void variantSetDefaultInterfaces()
{
    // add interfaces to interfacemanager

    interfaceManager::managedIf_t newInterface;

    // sx1262 loraInterface
    newInterface.name = "SX1262-loraInterface";

    newInterface.ifDescriptionType = interfaceManager::IF_SX1262;
    newInterface.ifDescription.radiolibDescription.radio = new SX1262(new Module(SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY));
    newInterface.ifDescription.radiolibDescription.tcxoVoltage = 1.6;
    newInterface.ifDescription.radiolibDescription.useRegulatorLDO = false;

    newInterface.ifConfigType = interfaceManager::CONFIG_LORA;
    newInterface.ifConfig.loraConfig.frequency = 869.5;
    newInterface.ifConfig.loraConfig.bandwidth = 125;
    newInterface.ifConfig.loraConfig.spreadingFactor = 9;
    newInterface.ifConfig.loraConfig.codingRate = 7;
    newInterface.ifConfig.loraConfig.syncWord = 18;
    newInterface.ifConfig.loraConfig.power = 10;

    newInterface.rnsIfMode = RNS::Type::Interface::modes::MODE_FULL;

    interfaceManager::addInterface(newInterface);

    // wifi interface
    newInterface.name = "wifi interface";

    newInterface.ifDescriptionType = interfaceManager::IF_UDP_WIFI;

    newInterface.ifConfigType = interfaceManager::CONFIG_UDP_WIFI;
    newInterface.ifConfig.wifiConfig.SSID = WIFI_SSID;
    newInterface.ifConfig.wifiConfig.KEY = WIFI_KEY;

    newInterface.rnsIfMode = RNS::Type::Interface::modes::MODE_FULL;

    interfaceManager::addInterface(newInterface);
}
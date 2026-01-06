#include "variant.h"
#include "interfaceManager.h"
#include "../../../credentials.h"

void variantSetDefaultInterfaces()
{
    // add interfaces to interfacemanager
    // sx1262 loraInterface
    interfaceManager::managedIf_t *loraInterface = new interfaceManager::managedIf_t;

    loraInterface->name = "SX1262-loraInterface";

    loraInterface->ifDescriptionType = interfaceManager::IF_RADIOLIB;
    loraInterface->ifDescription.radiolibDescription.radioType = radioLibInterface::RADIO_SX1262;
    loraInterface->ifDescription.radiolibDescription.radio = new SX1262(new Module(SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY));
    loraInterface->ifDescription.radiolibDescription.tcxoVoltage = 1.6;
    loraInterface->ifDescription.radiolibDescription.useRegulatorLDO = false;

    loraInterface->ifConfigType = interfaceManager::CONFIG_LORA;
    loraInterface->ifConfig.loraConfig.frequency = 869.5;
    loraInterface->ifConfig.loraConfig.bandwidth = 125;
    loraInterface->ifConfig.loraConfig.spreadingFactor = 9;
    loraInterface->ifConfig.loraConfig.codingRate = 7;
    loraInterface->ifConfig.loraConfig.syncWord = 18;
    loraInterface->ifConfig.loraConfig.power = 10;

    loraInterface->rnsIfMode = RNS::Type::Interface::modes::MODE_FULL;

    // transfer pointer ownership to interfaceManager
    interfaceManager::addInterface(loraInterface);

    // wifi interface
    interfaceManager::managedIf_t *wifiInterface = new interfaceManager::managedIf_t;

    wifiInterface->name = "wifi interface";

    wifiInterface->ifDescriptionType = interfaceManager::IF_UDP_WIFI;

    wifiInterface->ifConfigType = interfaceManager::CONFIG_UDP_WIFI;
    wifiInterface->ifConfig.wifiConfig.SSID = WIFI_SSID;
    wifiInterface->ifConfig.wifiConfig.KEY = WIFI_KEY;

    wifiInterface->rnsIfMode = RNS::Type::Interface::modes::MODE_FULL;

    // transfer pointer ownership to interfaceManager
    interfaceManager::addInterface(wifiInterface);
}
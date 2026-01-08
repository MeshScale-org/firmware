#include "variant.h"
#include "interfaceManager.h"
#include "../../../credentials.h"

#include "interfaces/radiolibInterface.h"
#include "interfaces/radiolibInterfaceAdapters/SX1262Adapter.h"

#include "interfaces/UDPInterface.h"

void variantSetDefaultInterfaces()
{
    // add interfaces to interfacemanager
    // sx1262 loraInterface
    interfaceManager::managedInterface_t *loraInterface = new interfaceManager::managedInterface_t;

    loraInterface->managedInterfaceConfig.ifType = managedInterfaceImpl_t::IF_RADIOLIB;
    loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.radioType = managedInterfaceImpl_t::RADIO_SX1262;

    loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemType = managedInterfaceImpl_t::CONFIG_LORA;
    loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.frequency = 869.5;
    loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.bandwidth = 125;
    loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.spreadingFactor = 9;
    loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.codingRate = 7;
    loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.syncWord = 18;
    loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.power = 10;

    loraInterface->managedInterfaceConfig.rnsIfMode = RNS::Type::Interface::modes::MODE_FULL;

    // TODO: replace by factory using managedInterfaceConfig.interfaceConfig.radiolibConfig.radioType (RADIO_SX1262)
    // loraInterface->managedInterfaceImpl = new radioLibInterface("sx1262 loraInterface", new SX1262(new Module(SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY)));
    loraInterface->managedInterfaceImpl = new radioLibInterface("sx1262 loraInterface", new SX1262Adapter(new SX1262(new Module(SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY))));

    // transfer pointer ownership to interfaceManager
    interfaceManager::addInterface(loraInterface);

    // wifi interface
    interfaceManager::managedInterface_t *wifiInterface = new interfaceManager::managedInterface_t;

    wifiInterface->managedInterfaceConfig.ifType = managedInterfaceImpl_t::IF_UDP;

    wifiInterface->managedInterfaceConfig.interfaceConfig.udpConfig.medium = managedInterfaceImpl_t::WIFI;
    wifiInterface->managedInterfaceConfig.interfaceConfig.udpConfig.network.SSID = WIFI_SSID;
    wifiInterface->managedInterfaceConfig.interfaceConfig.udpConfig.network.KEY = WIFI_KEY;

    wifiInterface->managedInterfaceConfig.rnsIfMode = RNS::Type::Interface::modes::MODE_FULL;

    wifiInterface->managedInterfaceImpl = new UDPInterface("Wifi Interface");

    // transfer pointer ownership to interfaceManager
    interfaceManager::addInterface(wifiInterface);
}
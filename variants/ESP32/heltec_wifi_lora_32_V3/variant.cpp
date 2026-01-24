#include "variant.h"
#include "main.h"
#include "handlerNetworkInterfaces.h"
#include "../../../credentials.h"

#include "networkInterfaces/radiolibInterface.h"
#include "networkInterfaces/radiolibInterfaceAdapters/SX1262Adapter.h"

#include "networkInterfaces/UDPInterface.h"

#include "os/concurrency/SPIClassL.h"

void variantSetDefaultInterfaces()
{

    // add interfaces to handlerNetworkInterfaces

    Serial.println("Setting up sx1262");
    // sx1262 loraInterface
    radioLimits_t radioLimits;
    radioLimits.minFreq = 863;
    radioLimits.maxFreq = 870;
    radioLimits.minPower = -9;
    radioLimits.maxPower = 22;

    managedInterfaceImpl_t::managedInterfaceConfig_t sx1262InterfaceConfig;
    sx1262InterfaceConfig.ifType = managedInterfaceImpl_t::IF_RADIOLIB;
    sx1262InterfaceConfig.interfaceConfig.radiolibConfig.radioType = managedInterfaceImpl_t::RADIO_SX1262;

    sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemType = managedInterfaceImpl_t::MODEM_LORA;
    sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.frequency = 869.5;
    sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.bandwidth = 125;
    sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.spreadingFactor = 9;
    sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.codingRate = 7;
    sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.syncWord = 0x42;
    sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.power = 5; // low power during testing
    sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.preambleLength = 16;

    sx1262InterfaceConfig.rnsIfMode = RNS::Type::Interface::modes::MODE_FULL;

    // interface ID, interface pair
    handlerNetworkInterfaces.addInterface(4, handlerNetworkInterfaces.createInterface("sx1262 loraInterface", sx1262InterfaceConfig, radioLimits, SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY, SPI0L));

    Serial.println("Setting up wifi IF");
    // wifi interface
    managedInterfaceImpl_t::managedInterfaceConfig_t udpInterfaceConfig;

    udpInterfaceConfig.ifType = managedInterfaceImpl_t::IF_UDP;

    udpInterfaceConfig.interfaceConfig.udpConfig.medium = managedInterfaceImpl_t::MEDIUM_WIFI;
    udpInterfaceConfig.interfaceConfig.udpConfig.network.SSID = WIFI_SSID;
    udpInterfaceConfig.interfaceConfig.udpConfig.network.KEY = WIFI_KEY;

    udpInterfaceConfig.rnsIfMode = RNS::Type::Interface::modes::MODE_FULL;

    handlerNetworkInterfaces.addInterface(7, handlerNetworkInterfaces.createInterface("Wifi Interface", udpInterfaceConfig));
}

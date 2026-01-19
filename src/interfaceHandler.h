#pragma once
#include <Arduino.h>
#include <vector>
#include <Reticulum.h>
#include <RadioLib.h>
#include "os/concurrency/SPIClassL.h"

#include "interfaces/managedInterfaceImpl.h"
#include "interfaces/radiolibInterfaceAdapters/radiolibInterfaceAdapter_base.h"

class interfaceHandler_t
{
public:
    struct managedInterface_t
    {
        managedInterfaceImpl_t::managedInterfaceConfig_t managedInterfaceConfig;
        managedInterfaceImpl_t *managedInterfaceImpl;
        RNS::Interface transportIf = RNS::Interface(RNS::Type::NONE);
    };

    // singleton interfaces
public:
    interfaceHandler_t(interfaceHandler_t &) = delete;

    bool addInterface(uint8_t ifID, managedInterface_t *newInterface, bool autoStart = true);
    // return true for succesfull config update and radio start
    bool configureInterface(uint8_t interfaceIndex, managedInterfaceImpl_t::managedInterfaceConfig_t newConfig);
    // gives overview of interfaces in interfaceHandler_t
    String interfacesToString(bool verbose = false);
    // register new interfaces to transport
    bool registerIfsTransport();
    // loop over interfaces and handle incoming packets ...
    void loop();

private:
    interfaceHandler_t() {};

public:
    // get singleton instance
    inline static interfaceHandler_t &getInstance()
    {
        static interfaceHandler_t instance;
        return instance;
    }

private:
    //  map for interface ID, interface object pairs
    std::map<uint8_t, managedInterface_t *> interfaces;

    // add interface / interface factories
public:
    // factory methods to create interfaces
    // returns interface or nullptr on fail

    // factory for radiolib interface
    managedInterface_t *createInterface(std::string ifName, managedInterfaceImpl_t::managedInterfaceConfig_t ifConfig, radioLimits_t radioLimits, uint32_t cs, uint32_t irq, uint32_t rst, uint32_t gpio, SPIClassL &spi, SPISettings spiSettings = RADIOLIB_DEFAULT_SPI_SETTINGS);

    // factory for UDP interface
    managedInterface_t *createInterface(std::string ifName, managedInterfaceImpl_t::managedInterfaceConfig_t ifConfig);
};

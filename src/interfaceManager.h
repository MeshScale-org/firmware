#pragma once
#include <Arduino.h>
#include <vector>
#include <Reticulum.h>
#include <RadioLib.h>
#include "os/concurrency/SPIClassL.h"

#include "interfaces/managedInterfaceImpl.h"
#include "interfaces/radiolibInterfaceAdapters/radiolibInterfaceAdapter_base.h"

class interfaceManager
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
    interfaceManager() {};
    interfaceManager(interfaceManager &) = delete;

    static bool addInterface(uint8_t ifID, managedInterface_t *newInterface, bool autoStart = true) { return get().addInterfaceImpl(ifID, newInterface, autoStart); };
    // return true for succesfull config update and radio start
    static bool configureInterface(uint8_t interfaceIndex, managedInterfaceImpl_t::managedInterfaceConfig_t newConfig) { return get().configureInterfaceImpl(interfaceIndex, newConfig); };
    // gives overview of interfaces in interfaceManager
    static String interfacesToString(bool verbose = false) { return get().interfacesToStringImpl(verbose); };
    // register new interfaces to transport
    static bool registerIfsTransport() { return get().registerIfsTransportImpl(); };
    // loop over interfaces and handle incoming packets ...
    static void loop() { return get().loopImpl(); };

private:
    bool addInterfaceImpl(uint8_t ifID, managedInterface_t *newInterface, bool autoStart);

private:
    // get singleton instance
    inline static interfaceManager &get()
    {
        static interfaceManager instance;
        return instance;
    }

    bool configureInterfaceImpl(uint8_t interfaceIndex, managedInterfaceImpl_t::managedInterfaceConfig_t newConfig);
    String interfacesToStringImpl(bool verbose);
    bool registerIfsTransportImpl();
    void loopImpl();

private:
    //  map for interface ID, interface object pairs
    std::map<uint8_t, managedInterface_t *> interfaces;

    // add interface / interface factories
public:
    // factory methods to create interfaces
    // returns interface or nullptr on fail

    // factory for radiolib interface
    static managedInterface_t *createInterface(std::string ifName, managedInterfaceImpl_t::managedInterfaceConfig_t ifConfig, radioLimits_t radioLimits, uint32_t cs, uint32_t irq, uint32_t rst, uint32_t gpio, SPIClassL &spi, SPISettings spiSettings = RADIOLIB_DEFAULT_SPI_SETTINGS);

    // factory for UDP interface
    static managedInterface_t *createInterface(std::string ifName, managedInterfaceImpl_t::managedInterfaceConfig_t ifConfig);
};

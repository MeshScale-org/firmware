#pragma once
#include <Arduino.h>
#include <vector>
#include <Reticulum.h>
#include <RadioLib.h>

// add interface classes
#include "interfaces/managedInterfaceImpl.h"

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
    static bool addInterface(uint8_t ifID, managedInterface_t *newInterface, bool autoStart = true) { return get().addInterfaceImpl(ifID, newInterface, autoStart); }
    static bool configureInterface(uint8_t interfaceIndex, managedInterfaceImpl_t::managedInterfaceConfig_t newConfig) { return get().configureInterfaceImpl(interfaceIndex, newConfig); };
    static String interfacesToString(bool verbose = false) { return get().interfacesToStringImpl(verbose); };
    static bool registerIfsTransport() { return get().registerIfsTransportImpl(); };
    static void loop() { return get().loopImpl(); };

private:
    // get singleton instance
    static interfaceManager &get()
    {
        static interfaceManager instance;
        return instance;
    }

    bool addInterfaceImpl(uint8_t ifID, managedInterface_t *newInterface, bool autoStart /*=true*/);
    bool configureInterfaceImpl(uint8_t interfaceIndex, managedInterfaceImpl_t::managedInterfaceConfig_t newConfig);
    String interfacesToStringImpl(bool verbose);
    bool registerIfsTransportImpl();
    void loopImpl();

private:
    //  map for interface ID, interface value pairs
    std::map<uint8_t, managedInterface_t *> interfaces;
};

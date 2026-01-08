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
        RNS::Interface RNS_IF = RNS::Interface(RNS::Type::NONE);
        bool update = false; // flag set to true if changes have been made to managedInterfaceConfig
    };

public:
    interfaceManager() {};
    interfaceManager(interfaceManager &) = delete;
    static bool addInterface(managedInterface_t *newInterface) { return get().addInterfaceImpl(newInterface); }
    static String interfacesToString(bool verbose = false) { return get().interfacesToStringImpl(verbose); };
    static bool updateTransportInterfaces() { return get().updateTransportInterfacesImpl(); };

private:
    // get singleton instance
    static interfaceManager &get()
    {
        static interfaceManager instance;
        return instance;
    }
    bool addInterfaceImpl(managedInterface_t *newInterface);
    String interfacesToStringImpl(bool verbose);
    bool updateTransportInterfacesImpl();

private:
    std::vector<managedInterface_t *> interfaces;
};

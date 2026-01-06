
#pragma once
#include <Reticulum.h>

class interfaceManager;

class rnsInterface_base : public RNS::InterfaceImpl
{

public:
    rnsInterface_base(const char *name) : RNS::InterfaceImpl(name) {};
    // virtual bool configManagedIf(interfaceManager::managedIf_t newConfig) const = 0;
};
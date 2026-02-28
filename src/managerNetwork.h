#pragma once
#include "manager_base.h"
#include "Reticulum.h"

/*
The network Manager

Jobs:


    ...
*/

class managerNetwork_t : public manager_base
{
public:
    managerNetwork_t(managerNetwork_t &) = delete;
    // get singleton instance
    inline static managerNetwork_t &getInstance()
    {
        static managerNetwork_t instance;
        return instance;
    };

private:
    managerNetwork_t() {};

public:
    unsigned long loop();

private:
    // our own destination to announce, send and receive with
    // RNS::Destination myDestination;
    const unsigned long networkInterval = 50;
};

extern managerNetwork_t &managerNetwork;
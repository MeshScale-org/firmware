#pragma once
#include "manager_base.h"
#include "Reticulum.h"

/*
The main Manager, distributes pbMessages

Jobs:
    Manage identity if local
    Spreading pbMessages
    Keep track of time
    Power saving/management
    Software watchdog, check if other managers/threads are working properly
    ...
*/

class managerSystem_t : public manager_base
{

public:
    managerSystem_t(managerSystem_t &) = delete;
    // get singleton instance
    inline static managerSystem_t &getInstance()
    {
        static managerSystem_t instance;
        return instance;
    }

    // class and thread related
private:
    managerSystem_t() {};
    const unsigned long systemInterval = 25;

public:
    unsigned long loop();

    // Jobs related
private:
    RNS::Bytes prv_bytes;
};

extern managerSystem_t &managerSystem;
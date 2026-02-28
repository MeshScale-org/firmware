#pragma once
#include "manager_base.h"
#include "Reticulum.h"

/*
The main Manager, distributes pbMessages

Jobs:
    manage our identity and dstination
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

private:
    managerSystem_t() {};

public:
    unsigned long loop();

private:
    const unsigned long systemInterval = 25;
};

extern managerSystem_t &managerSystem;
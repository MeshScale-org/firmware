#pragma once
#include "manager_base.h"

/*
The main Manager, distributes pbMessages

Jobs:
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
};

extern managerSystem_t &managerSystem;
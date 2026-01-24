#pragma once
#include "manager_base.h"
#include "rumor.h"
#include <queue>

/*
The main Manager, distributes rumors

Jobs:
    Spreading rumors
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
    void takeRumor(rumor_t newRumor) { rumorsIn.push(newRumor); };

private:
    std::queue<rumor_t> rumorsIn;
};
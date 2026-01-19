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

class systemManager_t : public manager_base
{

public:
    systemManager_t(systemManager_t &) = delete;
    // get singleton instance
    inline static systemManager_t &getInstance()
    {
        static systemManager_t instance;
        return instance;
    }

private:
    systemManager_t() {};

public:
    unsigned long loop();
    void takeRumor(rumor_t newRumor) { rumorsIn.push(newRumor); };

private:
    std::queue<rumor_t> rumorsIn;
};
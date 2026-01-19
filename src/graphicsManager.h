#pragma once
#include "manager_base.h"
#include "rumor.h"
#include <queue>

/*
The graphics Manager, manages all graphics and displays on the device

Jobs:
    handle:
    graphics,
    rendering,
    ...
*/

class graphicsManager_t : public manager_base
{

public:
    graphicsManager_t(graphicsManager_t &) = delete;
    // get singleton instance
    inline static graphicsManager_t &getInstance()
    {
        static graphicsManager_t instance;
        return instance;
    }

private:
    graphicsManager_t() {};

public:
    unsigned long loop();
    void takeRumor(rumor_t newRumor) { rumorsIn.push(newRumor); };

private:
    std::queue<rumor_t> rumorsIn;
};
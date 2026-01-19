#pragma once
#include "manager_base.h"
#include "rumor.h"
#include <queue>

/*
The hardware Manager, manages all device hardware (exluding interfaces and displays)

Jobs:
    Manage:
    sensors,
    user input,
    I2C scan,
    battery status
    ...
*/

class hardwareManager_t : public manager_base
{

public:
    hardwareManager_t(hardwareManager_t &) = delete;
    // get singleton instance
    inline static hardwareManager_t &getInstance()
    {
        static hardwareManager_t instance;
        return instance;
    }

private:
    hardwareManager_t() {};

public:
    unsigned long loop();
    void takeRumor(rumor_t newRumor) { rumorsIn.push(newRumor); };

private:
    std::queue<rumor_t> rumorsIn;
};
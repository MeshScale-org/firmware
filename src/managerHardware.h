#pragma once
#include "manager_base.h"
#include "rumor.h"
#include <queue>

/*
The hardware Manager, manages all device hardware (exluding network interfaces and user interfaces)

Jobs:
    Manage:
    sensors,
    I2C scan,
    battery status,
    ...
*/

class managerHardware_t : public manager_base
{

public:
    managerHardware_t(managerHardware_t &) = delete;
    // get singleton instance
    inline static managerHardware_t &getInstance()
    {
        static managerHardware_t instance;
        return instance;
    }

private:
    managerHardware_t() {};

public:
    unsigned long loop();
    void takeRumor(rumor_t newRumor) { rumorsIn.push(newRumor); };

private:
    std::queue<rumor_t> rumorsIn;
};
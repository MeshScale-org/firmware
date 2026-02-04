#pragma once
#include "manager_base.h"

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
};

extern managerHardware_t &managerHardware;
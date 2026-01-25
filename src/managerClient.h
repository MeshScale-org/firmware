#pragma once
#include "manager_base.h"
#include "rumor.h"
#include <queue>

/*
The client Manager, manages communication with client device (phone)

Jobs:
    communication with phone over BLE/Wifi
    ...
*/

class managerClient_t : public manager_base
{

public:
    managerClient_t(managerClient_t &) = delete;
    // get singleton instance
    inline static managerClient_t &getInstance()
    {
        static managerClient_t instance;
        return instance;
    }

private:
    managerClient_t() {};

public:
    unsigned long loop();
};

extern managerClient_t &managerClient;
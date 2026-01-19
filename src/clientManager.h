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

class clientManager_t : public manager_base
{

public:
    clientManager_t(clientManager_t &) = delete;
    // get singleton instance
    inline static clientManager_t &getInstance()
    {
        static clientManager_t instance;
        return instance;
    }

private:
    clientManager_t() {};

public:
    unsigned long loop();
    void takeRumor(rumor_t newRumor) { rumorsIn.push(newRumor); };

private:
    std::queue<rumor_t> rumorsIn;
};
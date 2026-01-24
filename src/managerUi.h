#pragma once
#include "manager_base.h"
#include "rumor.h"
#include <queue>

/*
The UI Manager, manages on-device interfaces(not network interfaces) to user like display and buttons

Jobs:
    handle:
    graphics,
    rendering,
    user button,
    rotary,
    keyboard,
    ...
*/

class managerUi_t : public manager_base
{

public:
    managerUi_t(managerUi_t &) = delete;
    // get singleton instance
    inline static managerUi_t &getInstance()
    {
        static managerUi_t instance;
        return instance;
    }

private:
    managerUi_t() {};

public:
    unsigned long loop();
    void takeRumor(rumor_t newRumor) { rumorsIn.push(newRumor); };

private:
    std::queue<rumor_t> rumorsIn;
};
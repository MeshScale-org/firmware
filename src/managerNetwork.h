#pragma once
#include "manager_base.h"
#include "rumor.h"
#include <queue>

/*
The reticulum Manager

Jobs:


    ...
*/

class managerNetwork_t : public manager_base
{
public:
    managerNetwork_t(managerNetwork_t &) = delete;
    // get singleton instance
    inline static managerNetwork_t &getInstance()
    {
        static managerNetwork_t instance;
        return instance;
    }

private:
    managerNetwork_t() {};

public:
    unsigned long loop();
    void takeRumor(rumor_t newRumor) { rumorsIn.push(newRumor); };

private:
    std::queue<rumor_t> rumorsIn;
    const unsigned long reticulumInterval = 1500;
};
#pragma once
#include "manager_base.h"
#include "rumor.h"
#include <queue>

/*
The reticulum Manager

Jobs:


    ...
*/

class reticulumManager_t : public manager_base
{
public:
    reticulumManager_t(reticulumManager_t &) = delete;
    // get singleton instance
    inline static reticulumManager_t &getInstance()
    {
        static reticulumManager_t instance;
        return instance;
    }

private:
    reticulumManager_t() {};

public:
    unsigned long loop();
    void takeRumor(rumor_t newRumor) { rumorsIn.push(newRumor); };

private:
    std::queue<rumor_t> rumorsIn;
    const unsigned long reticulumInterval = 1500;
};
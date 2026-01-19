#pragma once
#include "Arduino.h"
#include <functional>

#include "manager_base.h"

class thread
{
public:
    // generic thread constructor
    // thread(const std::string &name, std::function<unsigned long()> newCallback) : _name(name), callback(std::move(newCallback)) {}
    // managerThreadConstructor
    // never delete a manager that has a thread or callback will dissapear, might be safer with shared pointer to manager
    thread(const std::string &name, manager_base &Manager) : _name(name), callback(std::bind(&manager_base::loop, &Manager)) {};

protected:
#ifdef USE_RTOS
    static void runThread(void *thisThread);
#endif
    const std::string getName() { return _name; }

private:
    // called by scheduler
    virtual void runOnce()
    {
        lastRunTime = millis();
        timeTilNextRun = callback();
    };
    const std::string _name;

protected:
    unsigned long lastRunTime = 0;
    // next requested run time in millis, 0 to never call again
    unsigned long timeTilNextRun = 1; // set initial value to 1 so every thread is started on first run
private:
    std::function<unsigned long()> callback;
    friend class scheduler;
};
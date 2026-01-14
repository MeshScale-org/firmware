#pragma once

class thread
{
private:
    // called by scheduler
    virtual void runOnce() = 0;

protected:
    // next requested run time in millis, 0 to never call again
    unsigned long nextRunTime = 1; // set initial value to 1 so every thread is started on first run

    friend class scheduler;
};
#pragma once

#include "../os/concurrency/thread.h"
#include "../main.h"

class threadAnnounce : public thread
{
public:
    threadAnnounce(const char *name) : thread(name) {};

private:
    void runOnce();

private:
    const unsigned long announceInterval = 30000;
};
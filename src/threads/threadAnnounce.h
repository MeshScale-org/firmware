#pragma once

#include "../os/concurrency/thread.h"
#include "../main.h"

class threadAnnounce : public thread
{

private:
    void runOnce();

private:
    const unsigned long announceInterval = 30000;
};
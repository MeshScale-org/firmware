#pragma once

#include "../os/concurrency/thread.h"
#include "../main.h"

class threadPacket : public thread
{
public:
    threadPacket(const char *name) : thread(name) {};

private:
    void runOnce();

private:
    const unsigned long messageInterval = 10000;
};
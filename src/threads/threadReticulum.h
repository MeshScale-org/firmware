#pragma once
#include "../os/concurrency/thread.h"
#include "../main.h"

class threadReticulum : public thread
{
public:
    threadReticulum(const char *name) : thread(name) {};

private:
    void runOnce();

private:
    const unsigned long reticulumInterval = 200;
};
#pragma once
#include "../os/concurrency/thread.h"
#include "../main.h"

class threadReticulum : public thread
{
private:
    void runOnce();

private:
    const unsigned long reticulumInterval = 200;
};
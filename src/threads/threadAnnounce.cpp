#include "threadAnnounce.h"
#include <Arduino.h>

void threadAnnounce::runOnce()
{
    reticulum_announce();
    nextRunTime = millis() + announceInterval;
}

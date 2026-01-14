#include "threadReticulum.h"

void threadReticulum::runOnce()
{
    interfaceManager::loop(); // uses SPI to radio
    reticulum.loop();         // uses SPI to radio
    nextRunTime = millis() + reticulumInterval;
}
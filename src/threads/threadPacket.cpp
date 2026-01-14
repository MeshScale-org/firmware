#include "threadPacket.h"
#include <Arduino.h>

void threadPacket::runOnce()
{
    toggleLed();
    send_packet(); // uses SPI to radio
    nextRunTime = millis() + messageInterval;
}
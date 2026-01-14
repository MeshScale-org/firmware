#pragma once

#include "Arduino.h"
#include <Reticulum.h>
#include "interfaceManager.h"
#include <SPI.h>

// include threads
#include "threads/threadReticulum.h"
#include "threads/threadAnnounce.h"
#include "threads/threadPacket.h"

// Global instances
extern RNS::Reticulum reticulum;
extern RNS::FileSystem filesystem;
extern RNS::Identity identity;
extern RNS::Destination destination;

extern SPIClass SPI;

// global functions
void reticulum_announce();
void toggleLed();
void send_packet();
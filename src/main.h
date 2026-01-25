#pragma once

#include "Arduino.h"
#include <Reticulum.h>
#include "os/concurrency/SPIClassL.h"

// include managers and handlers
#include "managerSystem.h"
#include "managerNetwork.h"
#include "managerHardware.h"
#include "managerClient.h"
#include "managerUi.h"

#include "handlerNetworkInterfaces.h"

// Global RNS instances
extern RNS::Reticulum reticulum;
extern RNS::FileSystem filesystem;
extern RNS::Identity identity;
extern RNS::Destination destination;

extern RNS::Destination externDestination;

// used during setup
void setupSPI();
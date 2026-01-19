#pragma once

#include "Arduino.h"
#include <Reticulum.h>
#include "os/concurrency/SPIClassL.h"

// include managers and handlers
#include "systemManager.h"
#include "reticulumManager.h"
#include "hardwareManager.h"
#include "clientManager.h"
#include "graphicsManager.h"

#include "interfaceHandler.h"

// Global RNS instances
extern RNS::Reticulum reticulum;
extern RNS::FileSystem filesystem;
extern RNS::Identity identity;
extern RNS::Destination destination;

// Global Manager/Handler instances
extern systemManager_t &systemManager;
extern reticulumManager_t &reticulumManager;
extern hardwareManager_t &hardwareManager;
extern clientManager_t &clientManager;
extern graphicsManager_t &graphicsManager;

extern interfaceHandler_t &interfaceHandler;

extern RNS::Destination externDestination;

// used during setup
void setupSPI();
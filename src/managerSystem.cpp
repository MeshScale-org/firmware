#include "managerSystem.h"

unsigned long managerSystem_t::loop()
{
    return 0; // disable this thread
}

managerSystem_t &managerSystem = managerSystem_t::getInstance();
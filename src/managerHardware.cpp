#include "managerHardware.h"

unsigned long managerHardware_t::loop()
{
    return 0; // disable this thread
}

managerHardware_t &managerHardware = managerHardware_t::getInstance();
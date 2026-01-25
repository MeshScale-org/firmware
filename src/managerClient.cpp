#include "managerClient.h"

unsigned long managerClient_t::loop()
{
    return 0; // disable this thread
}

managerClient_t &managerClient = managerClient_t::getInstance();
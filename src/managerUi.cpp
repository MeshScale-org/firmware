#include "managerUi.h"

unsigned long managerUi_t::loop()
{
    return 0; // disable this thread
}

managerUi_t &managerUi = managerUi_t::getInstance();
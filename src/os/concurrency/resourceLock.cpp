#include "resourceLock.h"

#if USE_RTOS // for RTOS support

bool resourceLock::take()
{
    // TODO: implement
}

void resourceLock::release()
{
    // TODO: implement
}

#else // no RTOS, no locks needed

bool resourceLock::take()
{
    if (lockActive)
    {
        return false;
    }
    else
    {
        lockActive = true;
        return true;
    }
    return true;
}

void resourceLock::release()
{
    lockActive = false;
}

#endif

#pragma once

class resourceLock
{
public:
    bool take();
    void release();

private:
#if USE_RTOS
// lock object here
#else // should not be needed but for good practice
    bool lockActive = false;
#endif
};
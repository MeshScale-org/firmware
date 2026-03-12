#pragma once

#include <queue>
#include "resourceLock.h"

// lockable queue, calling function is responable for locking/unlocking
template <typename queueType>
class queueL : public resourceLock, public std::queue<queueType>
{
};

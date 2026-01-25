#pragma once

#include <queue>
#include "resourceLock.h"

// thread safe queue implementation
template <typename queueType>
class queueL : public resourceLock
{
public:
    void push(queueType newElement)
    {
        lock();
        _queue.push(newElement);
        unlock();
    };

    queueType front()
    {
        queueType ret;
        lock();
        ret = _queue.front();
        unlock();
        return ret;
    }

    void pop()
    {
        lock();
        _queue.pop();
        unlock();
    }

    bool empty()
    {
        bool ret;
        lock();
        ret = _queue.empty();
        unlock();
        return ret;
    }

private:
    std::queue<queueType> _queue;
};

#pragma once

#include "radiolibInterfaceAdapter_base.h"

class SX1262Adapter : public radiolibInterfaceAdapter_base
{
public:
    SX1262Adapter(SX1262 *radio) : radiolibInterfaceAdapter_base(radio) {};

private:
    SX1262 *radio;
};
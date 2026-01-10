#pragma once

#include "radiolibInterfaceAdapter_base.h"

class SX1262Adapter : public radiolibInterfaceAdapter_base
{
public:
    // both the SX1262* radio and the radiolibInterfaceAdapter_base PhysicalLayer* radio point at the same object
    SX1262Adapter(SX1262 *radio) : radiolibInterfaceAdapter_base(radio), radio(radio) {};

public:
    int16_t beginLora(float freq, float bw, uint8_t sf, uint8_t cr, uint8_t syncWord, int8_t power, uint16_t preambleLength);

    int16_t beginFSK(float freq, float br, float freqDev, float rxBw, int8_t power, uint16_t preambleLength);

private:
    SX1262 *radio;
};
#include "SX1262Adapter.h"

int16_t SX1262Adapter::beginLora(float freq, float bw, uint8_t sf, uint8_t cr, uint8_t syncWord, int8_t power, uint16_t preambleLength)
{
    return radio->begin(freq, bw, sf, cr, syncWord, power, preambleLength);
}

int16_t SX1262Adapter::beginFSK(float freq, float br, float freqDev, float rxBw, int8_t power, uint16_t preambleLength)
{
    return radio->beginFSK(freq, br, freqDev, rxBw, power, preambleLength);
}

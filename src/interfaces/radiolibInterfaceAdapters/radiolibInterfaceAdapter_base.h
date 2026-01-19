#pragma once
#include <RadioLib.h>

#include "os/concurrency/SPIClassL.h"

// TODO create proper namespace system
// hardware limits of a radio on a specific device/version
struct radioLimits_t
{
    float minFreq;
    float maxFreq;
    uint8_t minPower;
    uint8_t maxPower;
};

// All used radiolib radio-specific methods defined as virtual method here and returns RADIOLIB_ERR_UNSUPPORTED, unless overwrtitten
class radiolibInterfaceAdapter_base
{

public:
    radiolibInterfaceAdapter_base(PhysicalLayer *radio, radioLimits_t radioLimits, resourceLock &spiLock) : radio(radio), _radioLimits(radioLimits), radioSpiL(spiLock) {};
    ~radiolibInterfaceAdapter_base() { delete radio; };
    // methods required for radiolibInterface.cpp, methods of radiolib PhysicalLayer
public:
    int16_t startReceive();
    size_t getPacketLength(bool update = true);
    int16_t readData(uint8_t *data, size_t len);
    int16_t startTransmit(const uint8_t *data, size_t len, uint8_t addr = 0);
    int16_t finishTransmit();

    float getRSSI();
    float getSNR();
    bool receiveDone();
    bool transmitDone();

    // radio specific, should be overridden if supported by specific radio
public:
    virtual int16_t beginLora(float freq, float bw, uint8_t sf, uint8_t cr, uint8_t syncWord, int8_t power, uint16_t preambleLength) { return RADIOLIB_ERR_UNSUPPORTED; };

    virtual int16_t beginFSK(float freq, float br, float freqDev, float rxBw, int8_t power, uint16_t preambleLength) { return RADIOLIB_ERR_UNSUPPORTED; };

private:
    PhysicalLayer *radio;

protected:
    const radioLimits_t _radioLimits;
    resourceLock &radioSpiL;
};

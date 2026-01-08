#pragma once
#include <RadioLib.h>

// All used radiolib radio-specific methods defined as virtual method here and returns RADIOLIB_ERR_UNSUPPORTED, unless overwrtitten
class radiolibInterfaceAdapter_base
{

public:
    radiolibInterfaceAdapter_base(PhysicalLayer *radio) : radio(radio) {};
    // methods required for radiolibInterface.cpp, methods of radiolib PhysicalLayer
public:
    int16_t startReceive() { return radio->startReceive(); };
    size_t getPacketLength(bool update = true) { return radio->getPacketLength(); };
    int16_t readData(uint8_t *data, size_t len) { return radio->readData(data, len); };
    int16_t startTransmit(const uint8_t *data, size_t len, uint8_t addr = 0) { return radio->startTransmit(data, len, addr); };
    float getRSSI() { return radio->getRSSI(); };
    float getSNR() { return radio->getSNR(); };

    // radio specific, should be overridden if supported by specific radio
public:
    virtual int16_t beginLora() { return RADIOLIB_ERR_UNSUPPORTED; }
    virtual int16_t beginFSK() { return RADIOLIB_ERR_UNSUPPORTED; }

private:
    PhysicalLayer *radio;
};

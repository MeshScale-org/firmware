
#include "radiolibInterfaceAdapter_base.h"

#include <mutex>

int16_t radiolibInterfaceAdapter_base::startReceive()
{
    std::lock_guard<resourceLock> lg(radioSpiL);
    return radio->startReceive();
};

size_t radiolibInterfaceAdapter_base::getPacketLength(bool update)
{
    std::lock_guard<resourceLock> lg(radioSpiL);
    return radio->getPacketLength();
};

int16_t radiolibInterfaceAdapter_base::readData(uint8_t *data, size_t len)
{
    std::lock_guard<resourceLock> lg(radioSpiL);
    return radio->readData(data, len);
};

int16_t radiolibInterfaceAdapter_base::startTransmit(const uint8_t *data, size_t len, uint8_t addr)
{
    std::lock_guard<resourceLock> lg(radioSpiL);
    return radio->startTransmit(data, len, addr);
};

int16_t radiolibInterfaceAdapter_base::finishTransmit()
{
    std::lock_guard<resourceLock> lg(radioSpiL);
    return radio->finishTransmit();
};

float radiolibInterfaceAdapter_base::getRSSI()
{
    std::lock_guard<resourceLock> lg(radioSpiL);
    return radio->getRSSI();
};

float radiolibInterfaceAdapter_base::getSNR()
{
    std::lock_guard<resourceLock> lg(radioSpiL);
    return radio->getSNR();
};

bool radiolibInterfaceAdapter_base::receiveDone()
{
    std::lock_guard<resourceLock> lg(radioSpiL);
    bool res = radio->getIrqFlags() & RADIOLIB_SX126X_IRQ_RX_DONE;
    radio->clearIrq(RADIOLIB_SX126X_IRQ_RX_DONE);
    return res;
};

bool radiolibInterfaceAdapter_base::transmitDone()
{
    std::lock_guard<resourceLock> lg(radioSpiL);
    bool res = radio->getIrqFlags() & RADIOLIB_SX126X_IRQ_TX_DONE;
    radio->clearIrq(RADIOLIB_SX126X_IRQ_TX_DONE);
    return res;
};
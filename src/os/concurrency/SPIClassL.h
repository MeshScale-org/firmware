#pragma once

#include <SPI.h>
#include "resourceLock.h"

class SPIClassL : public resourceLock
{
public:
    SPIClassL(SPIClass &spi) : _spi(spi) {};
    SPIClass &get() { return _spi; };

private:
    SPIClass &_spi;
};

#if defined(MCU_NRF52)

#if SPI_INTERFACES_COUNT > 0
extern SPIClassL SPI0L;
#endif

#if SPI_INTERFACES_COUNT > 1
extern SPIClassL SPI1L;
#endif

#elif defined(MCU_ESP32)

extern SPIClassL SPI0L;

#endif

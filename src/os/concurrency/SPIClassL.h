#pragma once

#include <SPI.h>
#include "resourceLock.h"

// Lockable SPI class. Calling functions are responsable for locking/unlocking
class SPIClassL : public resourceLock, public SPIClass
{
public:
#if defined(MCU_ESP32)
    SPIClassL(uint8_t spi_bus) : SPIClass(spi_bus) {};
#elif defined(MCU_NRF52)
    SPIClassL(NRF_SPIM_Type *p_spi, uint8_t uc_pinMISO, uint8_t uc_pinSCK, uint8_t uc_pinMOSI) : SPIClass(p_spi, uc_pinMISO, uc_pinSCK, uc_pinMOSI) {};
#endif
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
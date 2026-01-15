#include "SPIClassL.h"

#if defined(MCU_NRF52)

#if SPI_INTERFACES_COUNT > 0
SPIClassL SPI0L = SPIClassL(SPI);
#endif

#if SPI_INTERFACES_COUNT > 1
SPIClassL SPI1L = SPIClassL(SPI1);
#endif

#elif defined(MCU_ESP32)

SPIClassL SPI0L = SPIClassL(SPI);

#endif
// define the locked SPI classes in the same way as the original SPI objects
// code copied from SPI.cpp for each platform

#include "SPIClassL.h"

#if defined(MCU_NRF52)

#if defined(NRF52840_XXAA) || defined(NRF52833_XXAA)
// default to 0
#ifndef SPI_32MHZ_INTERFACE
#define SPI_32MHZ_INTERFACE 0
#endif

#if SPI_32MHZ_INTERFACE == 0
#define _SPI_DEV NRF_SPIM3 // 32 Mhz
#define _SPI1_DEV NRF_SPIM2

#elif SPI_32MHZ_INTERFACE == 1
#define _SPI_DEV NRF_SPIM2
#define _SPI1_DEV NRF_SPIM3 // 32 Mhz

#else
#error "not supported yet"
#endif

#if SPI_INTERFACES_COUNT >= 1
// use SPIM3 for highspeed 32Mhz
SPIClassL SPI0L = SPIClassL(_SPI_DEV, PIN_SPI_MISO, PIN_SPI_SCK, PIN_SPI_MOSI);
#endif

#if SPI_INTERFACES_COUNT >= 2
SPIClassL SPI1L = SPIClassL(_SPI1_DEV, PIN_SPI1_MISO, PIN_SPI1_SCK, PIN_SPI1_MOSI);
#endif

#endif // defined(NRF52840_XXAA) || defined(NRF52833_XXAA)

#elif defined(MCU_ESP32)

#if CONFIG_IDF_TARGET_ESP32
SPIClassL SPI0L = SPIClassL(VSPI);
#else
SPIClassL SPI0L = SPIClassL(FSPI);
#endif

#endif

/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.
  Copyright (c) 2016 Sandeep Mistry All right reserved.
  Copyright (c) 2018, Adafruit Industries (adafruit.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "variant.h"
#include "wiring_constants.h"
#include "wiring_digital.h"
#include "nrf.h"

#include "interfaceManager.h"
#include "interfaces/radiolibInterface.h"
#include "interfaces/radiolibInterfaceAdapters/SX1262Adapter.h"

#include "os/concurrency/SPIClassL.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

  const uint32_t g_ADigitalPinMap[] =
      {
          // P0
          0, 1, 2, 3, 4, 5, 6, 7,
          8, 9, 10, 11, 12, 13, 14, 15,
          16, 17, 18, 19, 20, 21, 22, 23,
          24, 25, 26, 27, 28, 29, 30, 31,

          // P1
          32, 33, 34, 35, 36, 37, 38, 39,
          40, 41, 42, 43, 44, 45, 46, 47};

  void initVariant()
  {
    // LED1 & LED2
    pinMode(PIN_LED1, OUTPUT);
    ledOff(PIN_LED1);

    pinMode(PIN_LED2, OUTPUT);
    ledOff(PIN_LED2);
  }
#ifdef __cplusplus
}
#endif

// variant default interfaces

// SX1262 radio
#define SX126X_CS (42)
#define SX126X_DIO1 (47)
#define SX126X_BUSY (46)
#define SX126X_RESET (38)

void variantSetDefaultInterfaces()
{
  // add interfaces to interfacemanager
  // sx1262 loraInterface
  interfaceManager::managedInterface_t *loraInterface = new interfaceManager::managedInterface_t;

  loraInterface->managedInterfaceConfig.ifType = managedInterfaceImpl_t::IF_RADIOLIB;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.radioType = managedInterfaceImpl_t::RADIO_SX1262;

  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemType = managedInterfaceImpl_t::MODEM_LORA;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.frequency = 869.5;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.bandwidth = 125;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.spreadingFactor = 9;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.codingRate = 7;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.syncWord = 0x42;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.power = 5; // low power during testing
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.preambleLength = 0x16;

  loraInterface->managedInterfaceConfig.rnsIfMode = RNS::Type::Interface::modes::MODE_FULL;

  loraInterface->managedInterfaceImpl = new radioLibInterface("sx1262 loraInterface", SX126X_DIO1, SPI0L, new SX1262Adapter(new SX1262(new Module(SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY, SPI0L.get()))));

  // transfer pointer to interfaceManager
  interfaceManager::addInterface(0, loraInterface);
}

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
#include "nrf.h"
#include "wiring_constants.h"
#include "wiring_digital.h"

#include "interfaceManager.h"
#include "interfaces/radiolibInterface.h"
#include "interfaces/radiolibInterfaceAdapters/SX1262Adapter.h"

#include "os/concurrency/SPIClassL.h"

const uint32_t g_ADigitalPinMap[] = {
    // P0 - pins 0 and 1 are hardwired for xtal and should never be enabled
    0xff, 0xff, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,

    // P1
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};

// called by arduino
void initVariant()
{
  // LED1 & LED2
  pinMode(PIN_LED1, OUTPUT);
  ledOff(PIN_LED1);

  pinMode(PIN_LED2, OUTPUT);
  ledOff(PIN_LED2);

  pinMode(PIN_LED3, OUTPUT);
  ledOff(PIN_LED3);
}

void variantSetDefaultInterfaces()
{
  // add interfaces to interfacemanager

  Serial.println("Setting up sx1262");
  // sx1262 loraInterface
  radioLimits_t radioLimits;
  radioLimits.minFreq = 863;
  radioLimits.maxFreq = 870;
  radioLimits.minPower = -9;
  radioLimits.maxFreq = 22;

  managedInterfaceImpl_t::managedInterfaceConfig_t sx1262InterfaceConfig;
  sx1262InterfaceConfig.ifType = managedInterfaceImpl_t::IF_RADIOLIB;
  sx1262InterfaceConfig.interfaceConfig.radiolibConfig.radioType = managedInterfaceImpl_t::RADIO_SX1262;

  sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemType = managedInterfaceImpl_t::MODEM_LORA;
  sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.frequency = 869.5;
  sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.bandwidth = 125;
  sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.spreadingFactor = 9;
  sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.codingRate = 7;
  sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.syncWord = 0x42;
  sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.power = 5; // low power during testing
  sx1262InterfaceConfig.interfaceConfig.radiolibConfig.modemConfig.loraConfig.preambleLength = 16;

  sx1262InterfaceConfig.rnsIfMode = RNS::Type::Interface::modes::MODE_FULL;

  // add to interfaceManager
  interfaceManager::addInterface(3, interfaceManager::createInterface("sx1262 loraInterface", sx1262InterfaceConfig, radioLimits, SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY, SPI0L));
}
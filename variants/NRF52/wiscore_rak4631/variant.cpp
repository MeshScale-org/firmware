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
#include "interfaces/radioLibInterface.h"

const uint32_t g_ADigitalPinMap[] = {
    // P0
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,

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

  // 3V3 Power Rail
  pinMode(PIN_3V3_EN, OUTPUT);
  digitalWrite(PIN_3V3_EN, HIGH);
}

void variantSetDefaultInterfaces()
{
  // add interfaces to interfacemanager
  // sx1262 loraInterface
  interfaceManager::managedInterface_t *loraInterface = new interfaceManager::managedInterface_t;

  loraInterface->managedInterfaceConfig.ifType = managedInterfaceImpl_t::IF_RADIOLIB;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.radioType = managedInterfaceImpl_t::RADIO_SX1262;
  loraInterface->managedInterfaceImpl = new radioLibInterface("sx1262 loraInterface", new SX1262(new Module(SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY)));

  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.configType = managedInterfaceImpl_t::CONFIG_LORA;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.radiolibModemConfig.loraConfig.frequency = 869.5;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.radiolibModemConfig.loraConfig.bandwidth = 125;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.radiolibModemConfig.loraConfig.spreadingFactor = 9;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.radiolibModemConfig.loraConfig.codingRate = 7;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.radiolibModemConfig.loraConfig.syncWord = 18;
  loraInterface->managedInterfaceConfig.interfaceConfig.radiolibConfig.radiolibModemConfig.loraConfig.power = 10;

  loraInterface->managedInterfaceConfig.rnsIfMode = RNS::Type::Interface::modes::MODE_FULL;

  // transfer pointer to interfaceManager
  interfaceManager::addInterface(loraInterface);
}

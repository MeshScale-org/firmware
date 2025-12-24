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

  interfaceManager::managedIf_t newInterface;

  // sx1262 loraInterface
  newInterface.name = "SX1262-loraInterface";

  newInterface.ifDescriptionType = interfaceManager::IF_SX1262;
  newInterface.ifDescription.radiolibDescription.radio = new SX1262(new Module(SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY));
  newInterface.ifDescription.radiolibDescription.tcxoVoltage = 1.6;
  newInterface.ifDescription.radiolibDescription.useRegulatorLDO = false;

  newInterface.ifConfigType = interfaceManager::CONFIG_LORA;
  newInterface.ifConfig.loraConfig.frequency = 869.5;
  newInterface.ifConfig.loraConfig.bandwidth = 125;
  newInterface.ifConfig.loraConfig.spreadingFactor = 9;
  newInterface.ifConfig.loraConfig.codingRate = 7;
  newInterface.ifConfig.loraConfig.syncWord = 18;
  newInterface.ifConfig.loraConfig.power = 10;

  newInterface.rnsIfMode = RNS::Type::Interface::modes::MODE_FULL;

  interfaceManager::addInterface(newInterface);
}
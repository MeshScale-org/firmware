
#ifndef EXCLUDE_INTERFACE_RADIOLIB

#include "radiolibInterface.h"

#include "../src/Log.h"
#include "../src/Utilities/OS.h"

using namespace RNS;

radioLibInterface::radioLibInterface(std::string name, uint32_t irqPin, resourceLock &spi, radiolibInterfaceAdapter_base *radio) : managedInterfaceImpl_t(name), radioAdapter(radio)
{
	_IN = true;
	_OUT = true;
	// p self.bitrate = self.r_sf * ( (4.0/self.r_cr) / (math.pow(2,self.r_sf)/(self.r_bandwidth/1000)) ) * 1000
	_bitrate = 100; //(double)spreading * ((4.0 / coding) / (pow(2, spreading) / (bandwidth / 1000.0))) * 1000.0;
	_HW_MTU = 200;
	this->irqPin = irqPin;
}

/*virtual*/ radioLibInterface::~radioLibInterface()
{
	stop();
}

bool radioLibInterface::start()
{
	_online = false;
	INFO("Start receive...");
	Serial.printf("radio start receive with status code: %d\n", radioAdapter->startReceive());

	_online = true;
	return true;
}

void radioLibInterface::stop()
{
	_online = false;
}

void radioLibInterface::loop()
{
	if (_online)
	{
		// used as polling not irq
		if (irqPin && digitalRead(irqPin))
		{
			// Check for incoming packet
			if (radioAdapter->receiveDone())
			{
				receiveDone = 0;
				TRACE("radioLibInterface: receiving bytes...");

				// read header (for detecting split packets)
				// uint8_t header = radioAdapter->read();

				// read packet
				buffer.clear();

				// TODO: Can be optimized?
				uint16_t len = radioAdapter->getPacketLength();
				uint8_t *receivedBytes = new uint8_t[len];
				while (receivedBytes == nullptr)
				{
					Serial.println("bad alloc");
					delay(500);
				}
				radioAdapter->readData(receivedBytes, len);
				for (uint16_t i = 0; i < len; i++)
				{
					buffer << receivedBytes[i];
				}
				delete[] receivedBytes;

				Serial.println("RSSI: " + String(radioAdapter->getRSSI()));
				Serial.println("Snr: " + String(radioAdapter->getSNR()));

				on_incoming(buffer);
				Serial.println("###################################  Receving done!   ###################################");
				radioAdapter->startReceive();
			}
			if (radioAdapter->transmitDone())
			{
				radioAdapter->finishTransmit();
				Serial.println("###################################  Sending done!   ###################################");
				radioAdapter->startReceive();
			}
		}
	}
}

bool radioLibInterface::updateConfig(managedInterfaceImpl_t::managedInterfaceConfig_t newConfig)
{
	if (newConfig.ifType == managedInterfaceImpl_t::IF_RADIOLIB)
	{
		// to shorten member acces a bit
		auto &mdmCfg = newConfig.interfaceConfig.radiolibConfig.modemConfig;
		switch (newConfig.interfaceConfig.radiolibConfig.modemType)
		{

		case MODEM_NONE:
			Serial.println("radioLibInterface::updateConfig: ERROR: MODEM_NONE");
			return false;
			break;
		case MODEM_LORA:
			if (radioAdapter == nullptr)
			{
				Serial.println("radioLibInterface::updateConfig: radioAdapter is nullptr");
			}
			return (radioAdapter && radioAdapter->beginLora(mdmCfg.loraConfig.frequency, mdmCfg.loraConfig.bandwidth,
															mdmCfg.loraConfig.spreadingFactor, mdmCfg.loraConfig.codingRate, mdmCfg.loraConfig.syncWord,
															mdmCfg.loraConfig.power, mdmCfg.loraConfig.preambleLength) == RADIOLIB_ERR_NONE);
			break;
		case MODEM_FSK:
			if (radioAdapter == nullptr)
			{
				Serial.println("radioLibInterface::updateConfig: radioAdapter is nullptr");
			}
			return (radioAdapter && radioAdapter->beginFSK(mdmCfg.fskConfig.frequency, mdmCfg.fskConfig.bitRate,
														   mdmCfg.fskConfig.frequencyDeviation, mdmCfg.fskConfig.rxBandwidth, mdmCfg.fskConfig.power,
														   mdmCfg.fskConfig.preambleLength) == RADIOLIB_ERR_NONE);
			break;
		default:
			Serial.println("radioLibInterface::updateConfig: ERROR: modem not supported");
			return false;
			break;
		}
	}
	else
	{
		Serial.println("radioLibInterface::updateConfig was given a new config that is not meant for radiolibInterface");
		return false;
	}
}

/*virtual*/ void radioLibInterface::send_outgoing(const Bytes &data)
{

	DEBUG(toString() + ".on_outgoing: data: " + data.toHex());
	try
	{
		if (_online)
		{

			TRACE("radioLibInterface: sending " + std::to_string(data.size()) + " bytes...");
			//  Send packet

			// uint8_t *header = 0;
			//*header = (Cryptography::randomnum(256) & 0xF0);

			// radioAdapter->startTransmit(header, 1);

			// CBA TODO add support for split packets

			// add payload
			radioAdapter->startTransmit(data.data(), data.size());

			TRACE("radioLibInterface: sent bytes");
		}

		// Perform post-send housekeeping
		InterfaceImpl::handle_outgoing(data);
	}
	catch (std::exception &e)
	{
		ERROR("Could not transmit on " + toString() + ". The contained exception was: " + e.what());
	}
}

/*virtual*/ void radioLibInterface::on_incoming(const Bytes &data)
{
	DEBUG(toString() + ".on_incoming: data: " + data.toHex());
	// Pass received data on to transport
	InterfaceImpl::handle_incoming(data);
}

#endif
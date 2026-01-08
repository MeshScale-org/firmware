
#ifndef EXCLUDE_INTERFACE_RADIOLIB

#include "radiolibInterface.h"

#include "../src/Log.h"
#include "../src/Utilities/OS.h"

#include <memory>

using namespace RNS;

radioLibInterface::radioLibInterface(const char *name /*= "radioLibInterface"*/, radiolibInterfaceAdapter_base *radio) : radioAdapter(radio), managedInterfaceImpl_t(name)
{
	_IN = true;
	_OUT = true;
	// p self.bitrate = self.r_sf * ( (4.0/self.r_cr) / (math.pow(2,self.r_sf)/(self.r_bandwidth/1000)) ) * 1000
	_bitrate = 100; //(double)spreading * ((4.0 / coding) / (pow(2, spreading) / (bandwidth / 1000.0))) * 1000.0;
	_HW_MTU = 200;
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

		// Check for incoming packet
#ifdef ARDUINO
		if (receiveDone)
		{
			receiveDone = 0;
			TRACE("radioLibInterface: receiving bytes...");

			// read header (for detecting split packets)
			// uint8_t header = radioAdapter->read();

			// read packet
			buffer.clear();

			// TODO: Can be optimized
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
		}
#endif
	}
}

bool radioLibInterface::updateConfig(managedInterfaceImpl_t::managedInterfaceConfig_t rnsInterfaceDescription)
{
	if (rnsInterfaceDescription.ifType == managedInterfaceImpl_t::IF_RADIOLIB)
	{
		switch (rnsInterfaceDescription.interfaceConfig.radiolibConfig.modemType)
		{
		case CONFIG_NONE:
			return false;
			break;
		case CONFIG_LORA:
			return (radioAdapter->beginLora() == RADIOLIB_ERR_NONE);
			break;
		default:
			break;
		}

		return true;
	}

	return false;
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
#ifdef ARDUINO

			// uint8_t *header = 0;
			//*header = (Cryptography::randomnum(256) & 0xF0);

			// radioAdapter->startTransmit(header, 1);

			// CBA TODO add support for split packets

			// add payload
			radioAdapter->startTransmit(data.data(), data.size());

#endif
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
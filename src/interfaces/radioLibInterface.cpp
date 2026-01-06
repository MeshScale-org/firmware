
#ifndef EXCLUDE_INTERFACE_RADIOLIB

#include "radioLibInterface.h"

#include "../src/Log.h"
#include "../src/Utilities/OS.h"

#include <memory>

using namespace RNS;

radioLibInterface::radioLibInterface(const char *name /*= "radioLibInterface"*/, PhysicalLayer *radio) : radio(radio), rnsInterface_base(name)
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
	Serial.printf("radio start receive with status code: %d\n", radio->startReceive());

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
			// uint8_t header = radio->read();

			// read packet
			buffer.clear();

			// TODO: Can be optimized
			uint16_t len = radio->getPacketLength();
			uint8_t *receivedBytes = new uint8_t[len];
			while (receivedBytes == nullptr)
			{
				Serial.println("bad alloc");
				delay(500);
			}
			radio->readData(receivedBytes, len);
			for (uint16_t i = 0; i < len; i++)
			{
				buffer << receivedBytes[i];
			}
			delete[] receivedBytes;

			Serial.println("RSSI: " + String(radio->getRSSI()));
			Serial.println("Snr: " + String(radio->getSNR()));

			on_incoming(buffer);
		}
#endif
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
#ifdef ARDUINO

			// uint8_t *header = 0;
			//*header = (Cryptography::randomnum(256) & 0xF0);

			// radio->startTransmit(header, 1);

			// CBA TODO add support for split packets

			// add payload
			radio->startTransmit(data.data(), data.size());

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
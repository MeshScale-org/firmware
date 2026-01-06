#pragma once

#ifndef EXCLUDE_INTERFACE_RADIOLIB

#include "rnsInterface_base.h"

// radiolib
#include <RadioLib.h>

#ifdef ARDUINO
#include <SPI.h>
#endif

#include <stdint.h>

class radioLibInterface : public rnsInterface_base
{
public:
	enum radioTypes_t
	{
		RADIO_NONE,
		RADIO_SX1262
	};

public:
	// p def __init__(self, owner, name, device=None, bindip=None, bindport=None, forwardip=None, forwardport=None):
	radioLibInterface(const char *name, PhysicalLayer *radio);
	virtual ~radioLibInterface();

	virtual bool start();
	virtual void stop();
	virtual void loop();

	volatile bool sendDone = 0;
	volatile bool receiveDone = 0;

private:
	virtual void send_outgoing(const RNS::Bytes &data);
	void on_incoming(const RNS::Bytes &data);

private:
	RNS::Bytes buffer;
	PhysicalLayer *radio;
};

#endif
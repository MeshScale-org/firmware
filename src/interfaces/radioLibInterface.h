#pragma once

#ifndef EXCLUDE_INTERFACE_RADIOLIB

#include "managedInterfaceImpl.h"

// radiolib
#include <RadioLib.h>

#ifdef ARDUINO
#include <SPI.h>
#endif

#include <stdint.h>

class radioLibInterface : public managedInterfaceImpl_t
{
public:
	bool updateConfig(managedInterfaceImpl_t::managedInterfaceConfig_t rnsInterfaceDescription);

public:
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
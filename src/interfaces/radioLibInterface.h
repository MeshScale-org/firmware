#pragma once

#ifndef EXCLUDE_INTERFACE_RADIOLIB

#include "managedInterfaceImpl.h"

// radio adapter base class
#include "interfaces/radiolibInterfaceAdapters/radiolibInterfaceAdapter_base.h"

// radiolib
#include <RadioLib.h>

#ifdef ARDUINO
#include <SPI.h>
#endif

#include <stdint.h>

class radioLibInterface : public managedInterfaceImpl_t
{

public:
	radioLibInterface(const char *name, radiolibInterfaceAdapter_base *radio);
	virtual ~radioLibInterface();

	virtual bool start();
	virtual void stop();
	virtual void loop();

	volatile bool sendDone = 0;
	volatile bool receiveDone = 0;

public:
	bool updateConfig(managedInterfaceImpl_t::managedInterfaceConfig_t rnsInterfaceDescription);

private:
	virtual void send_outgoing(const RNS::Bytes &data);
	void on_incoming(const RNS::Bytes &data);

private:
	RNS::Bytes buffer;
	radiolibInterfaceAdapter_base *radioAdapter;
};

#endif

#pragma once

#ifndef EXCLUDE_INTERFACE_RADIOLIB

#include "managedInterfaceImpl.h"

// radio adapter base class
#include "interfaces/radiolibInterfaceAdapters/radiolibInterfaceAdapter_base.h"

// radiolib
#include <RadioLib.h>

#include <stdint.h>

class radioLibInterface : public managedInterfaceImpl_t
{

public:
	radioLibInterface(std::string name, uint32_t irqPin, resourceLock &radioSpiL, radiolibInterfaceAdapter_base *radio);
	virtual ~radioLibInterface();

	virtual bool start();
	virtual void stop();
	virtual void loop();

	volatile bool sendDone = 0;
	volatile bool receiveDone = 0;

public:
	bool updateConfig(managedInterfaceImpl_t::managedInterfaceConfig_t newConfig);

private:
	virtual void send_outgoing(const RNS::Bytes &data);
	void on_incoming(const RNS::Bytes &data);

private:
	RNS::Bytes buffer;
	radiolibInterfaceAdapter_base *radioAdapter;
	// would take irqPin from radiolib module but its protected/private
	// uint16_t irqPin = radioAdapter->getRadio()->getMod()->getIrq();
	uint32_t irqPin;
};

#endif

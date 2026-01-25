#pragma once

#ifndef EXCLUDE_INTERFACE_RADIOLIB

#include "managedInterfaceImpl.h"

// radio adapter base class
#include "networkInterfaces/radiolibInterfaceAdapters/radiolibInterfaceAdapter_base.h"

// radiolib
#include <RadioLib.h>

#include <stdint.h>

class radioLibInterface : public managedInterfaceImpl_t
{

public:
	radioLibInterface(std::string name, uint32_t irqPin, resourceLock &radioSpiL, radiolibInterfaceAdapter_base *radio);
	~radioLibInterface();

	bool start();
	void stop();
	void loop();

	volatile bool sendDone = 0;
	volatile bool receiveDone = 0;

	bool updateConfig(managedInterfaceImpl_t::managedInterfaceConfig_t newConfig);

private:
	void transmitOutQueue() override;

private:
	RNS::Bytes buffer;
	radiolibInterfaceAdapter_base *radioAdapter;
	// would take irqPin from radiolib module but its protected/private
	// uint16_t irqPin = radioAdapter->getRadio()->getMod()->getIrq();
	uint32_t irqPin;
};

#endif

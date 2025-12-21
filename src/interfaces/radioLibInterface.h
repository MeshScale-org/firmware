#pragma once

// reticulum
#include "../src/Interface.h"
#include "../src/Bytes.h"
#include "../src/Type.h"

// radiolib
#include <RadioLib.h>

#ifdef ARDUINO
#include <SPI.h>
#endif

#include <stdint.h>

/*
struct FSKConfig_t
{
};
struct LoraConfig_t
{
	float frequency = 0;
	float bandwidth = 0;
	uint8_t spreading = 0;
	uint8_t coding = 0;
	uint8_t power = 0;
};
struct LRFHSSConfig_t
{
};

union radioConfig_t
{
	FSKConfig_t FSKConfig;
	LoraConfig_t LoraConfig;
	LRFHSSConfig_t LRFHSSConfig;
};

struct radioLibInterfaceConfig
{
	ModemType_t modem = RADIOLIB_MODEM_NONE;
	radioConfig_t radioConfig;
};
*/
class radioLibInterface : public RNS::InterfaceImpl
{

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

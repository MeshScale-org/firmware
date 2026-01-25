#pragma once

#ifndef EXCLUDE_INTERFACE_UDP

#include "managedInterfaceImpl.h"

#include <WiFi.h>
#include <WiFiUdp.h>
// #include <AsyncUDP.h>

#include <stdint.h>

#include "../../credentials.h"

#define DEFAULT_UDP_PORT 4242
#define DEFAULT_UDP_LOCAL_HOST "192.168.1.14"
#define DEFAULT_UDP_REMOTE_HOST "192.168.1.255"

class UDPInterface : public managedInterfaceImpl_t
{

public:
	static const uint32_t BITRATE_GUESS = 10 * 1000 * 1000;

	// z def get_address_for_if(name):
	// z def get_broadcast_for_if(name):

public:
	bool updateConfig(managedInterfaceImpl_t::managedInterfaceConfig_t rnsInterfaceDescription);

public:
	// p def __init__(self, owner, name, device=None, bindip=None, bindport=None, forwardip=None, forwardport=None):
	UDPInterface(std::string name = "UDPInterface");
	virtual ~UDPInterface();

	// bool start(const char* wifi_ssid, const char* wifi_password, int port = DEFAULT_UDP_PORT, const char* local_host = nullptr);
	bool start();
	void stop();
	void loop();

	inline std::string toString() const { return "UDPInterface[" /* + _name + "/" + _local_host + ":" + std::to_string(_local_port) + "]"*/; }
	// virtual inline std::string toString() const { return "UDPInterface[" + name() + "]"; }
private:
	void transmitOutQueue() override;

private:
	// uint8_t buffer[Type::Reticulum::MTU] = {0};
	RNS::Bytes _buffer;

	// WiFi network name and password
	std::string _wifi_ssid;
	std::string _wifi_password;

	// IP address to send UDP data to.
	// it can be ip address of the server or
	// broadcast
	std::string _local_host = DEFAULT_UDP_LOCAL_HOST;
	int _local_port = DEFAULT_UDP_PORT;
	std::string _remote_host = DEFAULT_UDP_REMOTE_HOST;
	int _remote_port = DEFAULT_UDP_PORT;

	// create UDP instance
	WiFiUDP udp;
	// AsyncUDP udp;
};

#endif // defined ESP32

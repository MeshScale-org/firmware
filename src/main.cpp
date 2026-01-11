#include "Arduino.h"
#include "variant.h"

// includes
#include <RadioLib.h>
#include <Reticulum.h>
#include "interfaces/radiolibInterface.h"
#include "os/fileSystem.h"
#include "interfaceManager.h"

#ifndef EXCLUDE_INTERFACE_UDP
#include "interfaces/UDPInterface.h"
#endif

// SX1262
SX1262 radio = new Module(SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY);

// save transmission states between loops
int transmissionState = RADIOLIB_ERR_NONE;

// We initialise two lists of strings to use as app_data
const char *fruits[] = {"Peach", "Quince", "Date", "Tangerine", "Pomelo", "Carambola", "Grape"};
const char *noble_gases[] = {"Helium", "Neon", "Argon", "Krypton", "Xenon", "Radon", "Oganesson"};

double last_announce = 0.0;
bool send_announce = false;

// Test AnnounceHandler
class ExampleAnnounceHandler : public RNS::AnnounceHandler
{
public:
  ExampleAnnounceHandler(const char *aspect_filter = nullptr) : AnnounceHandler(aspect_filter) {}
  virtual ~ExampleAnnounceHandler() {}
  virtual void received_announce(const RNS::Bytes &destination_hash, const RNS::Identity &announced_identity, const RNS::Bytes &app_data)
  {
    Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    Serial.printf("ExampleAnnounceHandler: destination hash: %d\n", destination_hash.toHex());
    if (announced_identity)
    {
      Serial.printf("ExampleAnnounceHandler: announced identity hash: %d\n", announced_identity.hash().toHex());
      Serial.printf("ExampleAnnounceHandler: announced identity app data: %d\n", announced_identity.app_data().toHex());
    }
    if (app_data)
    {
      Serial.printf("ExampleAnnounceHandler: app data text: %s\n", app_data.toString());
    }
    Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  }
};

// Test packet receive callback
void onPacket(const RNS::Bytes &data, const RNS::Packet &packet)
{
  Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  Serial.printf("onPacket: data: %d\n", data.toHex());
  Serial.printf("onPacket: text: %s\n", data.toString());
  // TRACE("onPacket: " + packet.debugString());
  Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
}

// Ping packet receive callback
void onPingPacket(const RNS::Bytes &data, const RNS::Packet &packet)
{
  Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  Serial.printf("onPingPacket: data: %d\n", data.toHex());
  Serial.printf("onPingPacket: text: %s\n", data.toString());
  // TRACE("onPingPacket: " + packet.debugString());
  Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
}

RNS::Reticulum reticulum({RNS::Type::NONE});
RNS::Interface radioLib_interface(RNS::Type::NONE);
#ifndef EXCLUDE_INTERFACE_UDP
RNS::Interface udp_interface(RNS::Type::NONE);
#endif
RNS::FileSystem filesystem(RNS::Type::NONE);
RNS::Identity identity({RNS::Type::NONE});
RNS::Destination destination({RNS::Type::NONE});

radioLibInterface *radioLib_interface_impl = nullptr;
fileSystem *filesystem_impl = nullptr;

// ExampleAnnounceHandler announce_handler((const char*)"example_utilities.announcesample.fruits");
// RNS::HAnnounceHandler announce_handler(new ExampleAnnounceHandler("example_utilities.announcesample.fruits"));
RNS::HAnnounceHandler announce_handler(new ExampleAnnounceHandler());

void toggleLed()
{
  static bool ledState = 0;
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState);
}

void reticulum_announce()
{
  if (destination)
  {
    Serial.printf("Announcing destination...\n");
    // destination.announce(RNS::bytesFromString(fruits[RNS::Cryptography::randomnum() % 7]));
    //  test path
    // destination.announce(RNS::bytesFromString(fruits[RNS::Cryptography::randomnum() % 7]), true, nullptr, RNS::bytesFromString("test_tag"));
    //  test packet send
    destination.announce(RNS::bytesFromString(fruits[RNS::Cryptography::randomnum() % 7]));
  }
}

void reticulum_setup()
{
  INFO("Setting up Reticulum...");

  try
  {

    Serial.println("Registering FileSystem with OS...");
    filesystem_impl = new fileSystem();
    filesystem = filesystem_impl;
    filesystem.init();
    RNS::Utilities::OS::register_filesystem(filesystem);

    Serial.println("Creating Reticulum instance...");
    reticulum = RNS::Reticulum();
    reticulum.transport_enabled(true);
    reticulum.start();

    Serial.println("Creating Identity instance...");

    identity = RNS::Identity(false);
    RNS::Bytes prv_bytes;

    // static keys for testing
    // should be created/loaded from memory

#ifdef ESP32
    prv_bytes.assignHex("78E7D93E28D55871608FF13329A226CABC3903A357388A035B360162FF6321570B092E0583772AB80BC425F99791DF5CA2CA0A985FF0415DAB419BBC64DDFAE8");
#else
    prv_bytes.assignHex("E0D43398EDC974EBA9F4A83463691A08F4D306D4E56BA6B275B8690A2FBD9852E9EBE7C03BC45CAEC9EF8E78C830037210BFB9986F6CA2DEE2B5C28D7B4DE6B0");
#endif

    identity.load_private_key(prv_bytes);

    Serial.println("Creating Destination instance...");
    destination = RNS::Destination(identity, RNS::Type::Destination::IN, RNS::Type::Destination::SINGLE, "lxmf", "delivery");

    // Register DATA packet callback
    Serial.println("Registering packet callback with Destination...");
    destination.set_packet_callback(onPacket);
    destination.set_proof_strategy(RNS::Type::Destination::PROVE_ALL);

    {
      // Register PING packet callback
      Serial.println("Creating PING Destination instance...");
      RNS::Destination ping_destination(identity, RNS::Type::Destination::IN, RNS::Type::Destination::SINGLE, "example_utilities", "echo.request");

      Serial.println("Registering packet callback with PING Destination...");
      ping_destination.set_packet_callback(onPingPacket);
      ping_destination.set_proof_strategy(RNS::Type::Destination::PROVE_ALL);
    }

    Serial.println("Registering announce handler with Transport...");
    RNS::Transport::register_announce_handler(announce_handler);

    /*
        Serial.println("Announcing destination...");
        //destination.announce(RNS::bytesFromString(fruits[RNS::Cryptography::randomnum() % 7]));
        // test path
        //destination.announce(RNS::bytesFromString(fruits[RNS::Cryptography::randomnum() % 7]), true, nullptr, RNS::bytesFromString("test_tag"));
        // test packet send
        destination.announce(RNS::bytesFromString(fruits[RNS::Cryptography::randomnum() % 7]));
        // 23.9% (+0.8%)
    */

#if defined(RETICULUM_PACKET_TEST)
    // test data send packet
    Serial.println("Creating send packet...");
    RNS::Packet send_packet(destination, "The quick brown fox jumps over the lazy dog");

    Serial.println("Sending send packet...");
    send_packet.pack();
#ifndef NDEBUG
    TRACE("Test send_packet: " + send_packet.debugString());
#endif

    Serial.println("Creating recv packet...");
    RNS::Packet recv_packet({RNS::Type::NONE}, send_packet.raw());
    recv_packet.unpack();
#ifndef NDEBUG
    TRACE("Test recv_packet: " + recv_packet.debugString());
#endif

    Serial.println("Spoofing recv packet to destination...");
    destination.receive(recv_packet);
#endif

    Serial.println("Ready!");
  }
  catch (std::exception &e)
  {
    Serial.printf("!!! Exception in reticulum_setup: %s\n", e.what());
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

#ifdef ESP32
  SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI);
#else
  SPI.begin();
#endif

  variantSetDefaultInterfaces();
  delay(5000);

  Serial.print("Hello from device\n");
  Serial.print("Registering interfaces....\n");
  interfaceManager::registerIfsTransport();
  Serial.print("Registering done\n");

  reticulum_setup();

  // print out interface setup by variant
  delay(100);
  Serial.printf("################################\n%s\n################################\n", interfaceManager::interfacesToString(false).c_str());
  delay(500); // give print some time

  Serial.println("doing announce on lora");
  reticulum_announce();
  delay(10000);
  managedInterfaceImpl_t::managedInterfaceConfig_t newConfig;
  newConfig.ifType = managedInterfaceImpl_t::IF_RADIOLIB;
  newConfig.interfaceConfig.radiolibConfig.modemType = managedInterfaceImpl_t::MODEM_FSK;
  newConfig.interfaceConfig.radiolibConfig.modemConfig.fskConfig.frequency = 869.5;
  newConfig.interfaceConfig.radiolibConfig.modemConfig.fskConfig.bitRate = 4.8;
  newConfig.interfaceConfig.radiolibConfig.modemConfig.fskConfig.frequencyDeviation = 5;
  newConfig.interfaceConfig.radiolibConfig.modemConfig.fskConfig.rxBandwidth = 156.2;
  newConfig.interfaceConfig.radiolibConfig.modemConfig.fskConfig.power = 5; // low power during testing
  newConfig.interfaceConfig.radiolibConfig.modemConfig.fskConfig.preambleLength = 16;
  interfaceManager::configureInterface(0, newConfig);

  // print out interface setup by variant
  delay(100);
  Serial.printf("################################\n%s\n################################\n", interfaceManager::interfacesToString(true).c_str());
  delay(500); // give print some time
  Serial.println("doing announce on fsk");
  reticulum_announce();
  // reduce printouts after setup
  // RNS::loglevel(RNS::LOG_WARNING);
  RNS::loglevel(RNS::LOG_TRACE);

  Serial.println("end of setup()");
  delay(200);
}

unsigned long lastAnnounce = millis();
const unsigned long announceInterval = 30000;
void loop()
{

  reticulum.loop();
  interfaceManager::loop();

  // announce every interval time
  if (last_announce + announceInterval < millis())
  {
    toggleLed();
    last_announce = millis();
    // reticulum_announce();
  }

  delay(50);
}

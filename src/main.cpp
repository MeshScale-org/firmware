#include "main.h"
#include "variant.h"

#include <RadioLib.h>
#include "os/fileSystem.h"
#include "os/concurrency/scheduler.h"

#include <mutex>

// define managers/handlers
managerSystem_t &managerSystem = managerSystem_t::getInstance();
managerNetwork_t &managerNetwork = managerNetwork_t::getInstance();
managerHardware_t &managerHardware = managerHardware_t::getInstance();
managerClient_t &managerClient = managerClient_t::getInstance();
managerUi_t &managerUi = managerUi_t::getInstance();

handlerNetworkInterfaces_t &handlerNetworkInterfaces = handlerNetworkInterfaces_t::getInstance();

RNS::Destination externDestination = RNS::Destination(RNS::Type::NONE);

// Test AnnounceHandler
class ExampleAnnounceHandler : public RNS::AnnounceHandler
{
public:
  ExampleAnnounceHandler(const char *aspect_filter = nullptr) : AnnounceHandler(aspect_filter) {}
  virtual ~ExampleAnnounceHandler() {}
  virtual void received_announce(const RNS::Bytes &destination_hash, const RNS::Identity &announced_identity, const RNS::Bytes &app_data)
  {
    Serial.println("Setting external destination.....");
    externDestination = RNS::Destination(announced_identity, RNS::Type::Destination::IN, RNS::Type::Destination::SINGLE, destination_hash);
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
  TRACE("onPacket: " + packet.debugString());
  Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  RNS::Packet newPack(packet);
  newPack.unpack();

  TRACE("Test recv_packet: " + newPack.debugString());
}

// Ping packet receive callback
void onPingPacket(const RNS::Bytes &data, const RNS::Packet &packet)
{
  Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  Serial.printf("onPingPacket: data: %d\n", data.toHex());
  Serial.printf("onPingPacket: text: %s\n", data.toString());
  TRACE("onPingPacket: " + packet.debugString());
  Serial.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
}

RNS::Reticulum reticulum({RNS::Type::NONE});
RNS::FileSystem filesystem(RNS::Type::NONE);
RNS::Identity identity({RNS::Type::NONE});
RNS::Destination destination({RNS::Type::NONE});

fileSystem *filesystem_impl = nullptr;

// ExampleAnnounceHandler announce_handler((const char*)"example_utilities.announcesample.fruits");
// RNS::HAnnounceHandler announce_handler(new ExampleAnnounceHandler("example_utilities.announcesample.fruits"));
RNS::HAnnounceHandler announce_handler(new ExampleAnnounceHandler());

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
  // start serial for debugging
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  while (millis() < 7000)
  {
    Serial.print("Hello from device\n");
    delay(500);
  }

  Serial.println("SPIxL.begin()");
  // SPI begin
  setupSPI();

  Serial.println("Setting up default interfaces (variant)");
  // setup default interfaces
  variantSetDefaultInterfaces();
  delay(5000);

  // register known interface with reticulum transport
  Serial.print("Registering interfaces with rns transport....\n");
  handlerNetworkInterfaces.registerIfsTransport();

  // setup reticulum
  reticulum_setup();

  // print out interfaces setup by variant
  delay(100);
  Serial.printf("################################\n%s\n################################\n", handlerNetworkInterfaces.interfacesToString(true).c_str());
  delay(500); // give print some time

  // reduce printouts after setup
  RNS::loglevel(RNS::LOG_WARNING);
  // RNS::loglevel(RNS::LOG_TRACE);

  // create and register threads
  scheduler::addThread(new thread("managerSystem", managerSystem));
  scheduler::addThread(new thread("managerNetwork", managerNetwork));
  scheduler::addThread(new thread("managerHardware", managerHardware));
  scheduler::addThread(new thread("managerClient", managerClient));
  scheduler::addThread(new thread("managerUi", managerUi));

  Serial.println("end of setup()");
  delay(200);

#ifdef USE_RTOS
#ifdef MCU_ESP32
  // delete the setup/loop task and let scheduler run actual tasks
  vTaskDelete(NULL);
#else
// TODO add support for vanilla freeRTOS
#endif
#endif
}

void loop()
{
#ifdef USE_RTOS
  Serial.println("ERROR: RTOS enabled but loop() is running anyways");
  delay(500);
#else
  // use cooperative scheduler
  scheduler::runCoOp();
#endif
}

void setupSPI()
{
  std::lock_guard<resourceLock> lg(SPI0L);
#ifdef ESP32
  SPI0L.get().begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI);
#else
  SPI0L.get().begin();
#endif
}
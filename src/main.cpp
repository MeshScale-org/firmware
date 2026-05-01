#include "main.h"
#include "variant.h"

#include <RadioLib.h>
#include <microStore/FileSystem.h>
#include <microStore/Adapters/UniversalFileSystem.h>
#include <Utilities/OS.h>
#include "os/concurrency/scheduler.h"

#include <mutex>

RNS::Reticulum reticulum({RNS::Type::NONE});
RNS::Identity identity({RNS::Type::NONE});

void reticulum_setup()
{
  INFO("Setting up Reticulum...");

  try
  {
    // Initialize and register filesystem
    HEAD("Registering FileSystem with OS...", RNS::LOG_TRACE);
    microStore::FileSystem filesystem{microStore::Adapters::UniversalFileSystem()};
    filesystem.init();
    RNS::Utilities::OS::register_filesystem(filesystem);

    Serial.println("Creating Reticulum instance...");
    reticulum = RNS::Reticulum();
    reticulum.transport_enabled(true);
    reticulum.start();

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
  // some gibberish printed after the %s for some reason (on nrf52)
  Serial.printf("################################\n %s \n################################\n", handlerNetworkInterfaces.interfacesToString(true).c_str());
  delay(500); // give print some time

  // reduce printouts after setup
  RNS::loglevel(RNS::LOG_WARNING);
  // RNS::loglevel(RNS::LOG_TRACE);

  // create and register threads
  Serial.printf("Now creating threads.....\n");
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
  SPI0L.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI);
#else
  SPI0L.begin();
#endif
}
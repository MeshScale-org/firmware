# MeshScale firmware

The MeshScale firmware runs on devices that are supported by the arduino framework. The firmware can use interfaces on the device to create a mesh network with other devices using the reticulum network stack.

The firmware is still very much a work in progress, so a building and flashing guide is not yet available. If you want more info, let me know. (meshscale@mailfence.com, https://discord.gg/QDeTQHhZ)


## Overview of the firmware dependencies

The firmware is Arduino-based and is meant to support a large range of devices. Here is an overview of the major dependencies and their function:

- [microReticulum](https://github.com/attermann/microReticulum): The C++ port of the [Reticulum Network Stack](https://reticulum.network/) that holds the meshing logic.
- [PlatformIO](https://platformio.org/): The toolchain that makes developing and compiling to many different hardware types possible.
- [nanoPb](https://github.com/nanopb/nanopb): The low-footprint implementation of protocol buffers. Used to standardise and serialise data.
- [RadioLib](https://github.com/jgromes/RadioLib): The hardware abstraction layer for many radio types.

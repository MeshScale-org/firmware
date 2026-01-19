#pragma once
#include "Arduino.h"
#include <vector>

/*
A rumor is internal information passed between managers.
*/

enum rumorType_t
{
    RUMOR_NONE,
    RUMOR_TIME,
    RUMOR_BATTERY_LEVEL
};

#pragma region rumorTypes

// RUMOR_TIME
enum timeQuality_t
{
    NONE,
    MESH,   // received over mesh
    CLIENT, // received from client
    RTC,
    GPS
};
struct RUMOR_TIME_t
{
    timeQuality_t timeQuality = NONE;
    uint64_t time; // millis since epoch, should be large enough for now
};

// RUMOR_BATTERY_LEVEL
struct RUMOR_BATTERY_LEVEL_t
{
    uint8_t percentage = 0;
};

#pragma endregion rumorTypes

struct rumor_t
{
    rumorType_t rumorType = RUMOR_NONE;
    std::vector<uint8_t> data;
};

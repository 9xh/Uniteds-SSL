#pragma once
#pragma once

#include "Utils/storage.h"

constexpr bool SendLogsToConsole = false;
constexpr bool WritePEToLogs = false;
constexpr bool INGAME = true;
const char* FNhost = "storm-dev1.herokuapp.com"; // TODO: Check if fnhost can be a string (No reason for it to error but idk) and check if encryption works with it.
constexpr int vProd = 17;
constexpr int vS13 = 13;
constexpr int Version = vProd;
constexpr bool bIsHybrid = true;
HookEnum HookMethod = HookEnum::KIERO;

// DEFINES

// #define READFILE

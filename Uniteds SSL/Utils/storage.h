#pragma once

enum class HookEnum {
	VEH = 0, // VectoredExceptionHandler
	MH = 1, // MinHook
	KIERO = 2 // DX9, DX10, Vulkan, DX11, DX12, OpenGl, etc.
};

inline bool InitVEH = false;
inline bool InitMH = false;
inline bool InitKiero = false;

/* Error codes for United SSL

0-99:
Minhook Errors
100-199:
Cobalt Errors
200-299:
Kiero Errors
300-399

*/

enum class Errors
{
	BOOL_TO_STR_FAILED = 100,
	FSTREAM_NOT_OPEN = 200,
	KIERO_NOT_INITIALIZED = 300,
	KIERO_UNKNOWN_ERROR = 399
};

int GetErr(Errors err)
{
	if (err == Errors::BOOL_TO_STR_FAILED) return 100;
	else if (err == Errors::BOOL_TO_STR_FAILED) return 200;
	else if (err == Errors::KIERO_NOT_INITIALIZED) return 300;
	else if (err == Errors::KIERO_UNKNOWN_ERROR) return 399;
	return 10000;
}
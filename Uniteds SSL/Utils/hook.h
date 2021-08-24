// hook.h - Contains Diffrent Hooking Methods/Setup

#pragma once

#include "storage.h"
#include "../Third Party/MinHook/MinHook.h"
#include "logs.h"
#include "../Third Party/Kiero/kiero.h"
#include "../Third Party/xorstr.hpp"
using namespace Logs;

class Hooking
{
private:
	static bool Init(HookEnum type)
	{
		switch (type)
		{
		case HookEnum::VEH:
			//AddVectoredExceptionHandler(true, (PVECTORED_EXCEPTION_HANDLER)VectoredExceptionHandler);
			InitVEH = true;
			return InitVEH;
			break;
		case HookEnum::MH:
			if (InitMH) return true;
			MH_Initialize();
			InitMH = true;
			return InitMH;
			break;
		case HookEnum::KIERO:
			bool init_hook = false;
			do
			{
				if (kiero::init(kiero::RenderType::Auto) == kiero::Status::Success)
				{
					init_hook = true;
				}
			} while (!init_hook);
			InitKiero = true;
			return InitKiero;
			break;
		}
		return false;
	}
public:
	static bool Hook(void* Dest, void* detour, void* og, HookEnum type)
	{
		if (!Init(type)) Init(type);

		switch (type) {
		case HookEnum::VEH:
			//if (AreSamePage(Dest, detour)) return false;
			//DWORD dwOldProtect;
			//VirtualProtect(Dest, 1, PAGE_EXECUTE_READ | PAGE_GUARD, &dwOldProtect);
			//Hooks.emplace_back(Dest, detour);
			return true;
		case HookEnum::MH:
			MH_STATUS status;
			status = MH_CreateHook(Dest, detour, (void**)og);
			if (status != MH_OK)
			{
				std::string StatusStr = MH_StatusToString(status);
				MessageBoxA(0, xor ("Error while hooking!"), xor ("United SSL Fatal Error!"), MB_ICONERROR);
				if (status == MH_ERROR_NOT_INITIALIZED)
				{
					Log(xor ("Somehow, Minhook was not initialized. The builder of this dll can add \"MH_Initialize();\" at the beginning of the hook function."));
					// TODO: Create Exit Function.
				}
				Log(xor ("MH Error (report this to devs): ") + StatusStr); // TOOD: Add better error handling, with maybe solutions for the user.
				return false;
			}
			return true;
			break;
		case HookEnum::KIERO:
			kiero::Status::Enum a = kiero::bind(Dest, (void**)og, detour);
			if (a != kiero::Status::Success)
			{
				if (a == kiero::Status::NotInitializedError)
				{
					Logs::LogInt(GetErr(Errors::KIERO_NOT_INITIALIZED), Colors::defaultGray, true, true);
				}
				return false;
			}
		}
		return false;
	}
	bool Unhook(void* Dest) // TODO
	{
		return NULL;
	}
};
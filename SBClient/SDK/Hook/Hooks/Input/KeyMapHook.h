#pragma once
#include "../../../../Utils/MemoryUtils.h"

class KeyMapHook {
protected:
	using func_t = __int64(__fastcall*)(uint64_t, bool);
	static inline func_t func;

	static __int64 KeyPress(uint64_t key, bool isDown) {

		//Logger::ImLogF("KeyMapHooked\n");
		return func(key, isDown);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::Hooks::KeyMapHook);
		Logger::ImLogF((char*)address);
		MemoryUtils::CreateHook("KeyMapHook", address, (void*)&KeyMapHook::KeyPress, (void*)&func);
	}
};

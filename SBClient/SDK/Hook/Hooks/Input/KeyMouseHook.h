#pragma once
#include "../../../../Utils/MemoryUtils.h"



class KeyMouseHook {
protected:
	using func_t = void(__fastcall*)(__int64, char, char, __int16, __int16, __int16, __int16, char);
	static inline func_t func;

	static void clickFunc(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {
		func(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::Hooks::KeyMouseHook);
		Logger::ImLogF((char*)address);
		MemoryUtils::CreateHook("KeyMouseHook", address, (void*)&KeyMouseHook::clickFunc, (void*)&func);
	}
};


/**/
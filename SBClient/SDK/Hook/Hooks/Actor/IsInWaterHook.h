#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../World/Actor.h"

class IsInWaterHook {
protected:
	using func_t = bool(__fastcall*)(Actor*);
	static inline func_t func;

	static bool Actor_IsInWaterHook(Actor* _this) {
		if (_this->isInWater()) {
			Logger::ImLogF("InWater\n");
		}
		//return true;
		return func(_this);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::Actor::IsInWaterHook);
		Logger::ImLogF((char*)address);
		MemoryUtils::CreateHook("Actor_IsInWaterHook", address, (void*)&IsInWaterHook::Actor_IsInWaterHook, (void*)&func);
	}
};
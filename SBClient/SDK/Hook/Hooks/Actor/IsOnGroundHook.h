#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../World/Actor.h"

class IsOnGroundHook {
protected:
	using func_t = bool(__fastcall*)(Actor*);
	static inline func_t func;

	static bool Actor_IsOnGroundHook(Actor* _this) {
		if (_this->isOnGround()) {
			Logger::ImLogF("OnGround\n");
		}
		//return true;
		return func(_this);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::Actor::IsOnGroundHook);
		Logger::ImLogF((char*)address);
		MemoryUtils::CreateHook("Actor_isOnGroundHook", address, (void*)&IsOnGroundHook::Actor_IsOnGroundHook, (void*)&func);
	}
};

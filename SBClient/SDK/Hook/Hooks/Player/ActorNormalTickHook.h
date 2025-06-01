#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/GameData.h"

class ActorNormalTickHook {
protected:
	using func_t = void(__fastcall*)(Actor*);
	static inline func_t func;

	static void callback(Actor* _this) {
		//if()
		//if(clientInstance = nullptr)
		if (_this == mc.getLocalPlayer())
			Logger::ImLogF("LocalPlayer NormalTick");
			//client->moduleMgr->onNormalTick(_this);
		func(_this);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::Actor::ActorNormalTickHook);
		Logger::ImLogF((char*)address);
		MemoryUtils::CreateHook("Actor_NormalTickHook", address, (void*)&ActorNormalTickHook::callback, (void*)&func);
	}
};

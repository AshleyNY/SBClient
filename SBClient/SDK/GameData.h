#pragma once
#include "Core/ClientInstance.h"
class GameData {

private:
	ClientInstance* clientInstance = nullptr;
	//HIDController* hidController = nullptr;
public:
	inline ClientInstance* getClientInstance() { return this->clientInstance; }
	inline void setClientInstance(ClientInstance* ci) { this->clientInstance = ci; }
	inline LocalPlayer* getLocalPlayer() {
		if (clientInstance == nullptr)
			return 0;
		else
			return clientInstance->getLocalPlayer();
	}
};

extern GameData mc;
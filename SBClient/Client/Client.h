#pragma once
#include "../Utils/Logger.h"

#include "../Utils/Utils.h"
class Client {
private:
	bool initialized = false;
public:
	inline bool isInit() const {
		return initialized;
	}
	void Init();
	~Client();
};

extern Client* client;
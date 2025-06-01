#include "Client.h"
#include "../SDK/Hook/HookManager.h"


void Client::Init() {
    std::string clientPath = Utils::getClientPath();
    if (!Utils::doesClientPathExist(clientPath)) {
        Utils::createPath(clientPath);
    }
    std::string configsPath = clientPath + "Configs\\";
    if (!Utils::doesClientPathExist(configsPath)) {
        Utils::createPath(configsPath);
    }

    HookManager::Init();

    this->initialized = true;

}

Client::~Client() {
    HookManager::Restore();
    initialized = false;
}

Client* client = new Client();
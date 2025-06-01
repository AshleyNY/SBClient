#include <Windows.h>
#include "Libs/MinHook/MinHook.h"
#include "Libs/Libhat/libhat.hpp"
#include "Client/Client.h"
#include "../../SDK/Hook/Hooks/ImGuiHook/ImGuiHook.h"


DWORD WINAPI initClient(LPVOID lpParameter) {


    client->Init();


    while (true) {
      
        Sleep(25);
    }

    delete client;
    FreeLibraryAndExitThread((HMODULE)lpParameter, 1);
    ExitThread(0);
}

BOOL APIENTRY DllMain( HMODULE module,
                       DWORD  reason,
                       LPVOID lpReserved
                     )
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
          

            //创建线程，并初始化客户端
            CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)initClient, module, 0, nullptr);
            Logger::Init();
            break;
        case DLL_PROCESS_DETACH:
            client->~Client();
            Logger::Shutdown();
            break;
    }

    return TRUE;
}


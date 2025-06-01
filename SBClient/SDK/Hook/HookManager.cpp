#include "HookManager.h"
#include "Hooks/Input/KeyMapHook.h"
#include "Hooks/Input/KeyMouseHook.h"
#include "Hooks/Render/SetUpAndRenderHook.h"
#include "Hooks/Render/LevelRenderHook.h"
#include "Hooks/Actor/IsOnGroundHook.h"
#include "Hooks/Actor/IsInWaterHook.h"
#include "Hooks/Player/ActorNormalTickHook.h"
#include "Hooks/ImGuiHook/ImGuiHook.h"


void HookManager::Init() {
	MH_Initialize();
	OutputDebugStringA("DLL_PROCESS_ATTACH");

	ImGuiHook::Init();
	OutputDebugStringA("InitGui");

	//Render
	SetUpAndRenderHook::init();
	RenderLevelHook::init();

	//Input
	KeyMapHook::init();
	KeyMouseHook::init();

	//Actor
	IsInWaterHook::init();
	IsOnGroundHook::init();
	ActorNormalTickHook::init();
	//Component

	//Vtable
	{
		uintptr_t** PlayerVTable = MemoryUtils::getVtableFromSignature(Sigs::Vtables::PlayerVtable, 3);//offset param is 3 forever

	}
	
}

void HookManager::Restore() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	ImGuiHook::Shutdown();
	MH_Uninitialize();
}
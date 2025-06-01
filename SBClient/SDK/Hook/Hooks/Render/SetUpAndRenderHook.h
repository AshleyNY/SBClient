#pragma once
#include "../../Utils/Math.h"
#include "../../../Render/ScreenView.h"
#include "../../../Render/MinecraftUIRenderContext.h"
#include "../../../GameData.h"

Vec2<float> titlePos = { 0,0 };
float anim = 0;
Vec2<float> titleSize = { 0,0 };
namespace GuiInfo
{
	static float DeltaTime = 0.016f;
	static float GuiScale = 1;

	static Vec2<float> TrueScreenRes = { 0, 0 };
	static Vec2<float> ScreenRes = { 0, 0 };

	static Vec2<short> TrueMousePos = { 0, 0 };
	static Vec2<short> MousePos = { 0, 0 };
}
class SetUpAndRenderHook {
protected:
	using func_t = __int64(__fastcall*)(ScreenView*, MinecraftUIRenderContext*);
	static inline func_t func;

	static __int64 SetUpAndRender(ScreenView* _this, MinecraftUIRenderContext* renderCtx) {
		mc.setClientInstance(renderCtx->clientInstance);

		
			uintptr_t* visualTree = *(uintptr_t**)((uintptr_t)(_this)+0x48);
			std::string rootControlName = *(std::string*)((uintptr_t)(visualTree)+0x28);
			uintptr_t* CtxVTable = *(uintptr_t**)renderCtx;

		__int64 result = func(_this, renderCtx);

		return result;
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::Hooks::SetUpAndRenderHook);
		Logger::ImLogF((char*)address);
		MemoryUtils::CreateHook("SetUpAndRenderHook", address, (void*)&SetUpAndRenderHook::SetUpAndRender, (void*)&func);
	}
};
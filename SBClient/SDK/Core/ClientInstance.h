#pragma once
#include "../../Utils/MemoryUtils.h"
#include "MinecraftGame.h"
#include "GuiData.h"
#include "Minecraft.h"
#include "../Render/LevelRender.h"
#include "../World/LocalPlayer.h"

class ClientInstance {
public:
	BUILD_ACCESS(this, MinecraftGame*, minecraftGame, 0xC8);
	BUILD_ACCESS(this, GuiData*, guiData, 0x5B8);//1464
	BUILD_ACCESS(this, Minecraft*, minecraft, 0xD0);

	LevelRenderer* getLevelRenderer() {
		// IDA: search for string "variable.player_x_rotation" -> xrefs first function -> find what function have 4 param and access with a3 -> what acces a3
		return MemoryUtils::CallVFunc<214, LevelRenderer*>(this);
	}

	LocalPlayer* getLocalPlayer() {
		return MemoryUtils::CallVFunc<32, LocalPlayer*>(this);
	}

};


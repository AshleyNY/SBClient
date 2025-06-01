#pragma once
#include "../../Utils/MemoryUtils.h"
#include "Tessellator.h"
//create by SadAmI 2025/5/30
class ScreenContext {
public:
	//BUILD_ACCESS(this,mce::Camera*, camera, 0x18);
	BUILD_ACCESS(this, float*, currentShaderColor, 0x30);//1.21.82万能不变
	BUILD_ACCESS(this,Tessellator*, tessellator, 0xC8);//1.21.82
};
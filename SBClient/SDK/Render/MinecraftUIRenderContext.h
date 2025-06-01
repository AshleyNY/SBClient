#pragma once
#include "../Core/ClientInstance.h"
#include "ScreenContext.h"

#include "Font.h"
#include "TextMeasureData.h"
#include "../../Utils/ColorUtils.h"

class MinecraftUIRenderContext {
public:
	ClientInstance* clientInstance;
	ScreenContext* screenContext;
public:
	virtual ~MinecraftUIRenderContext();
	virtual float getLineLength(Font* font, std::string * str, float textSize, bool isCalcWidth);
	virtual float getTextAlpha();
	virtual void setTextAlpha(float alpha);
	virtual __int64 drawDebugText(const float* pos, std::string* text, float* color, float alpha, unsigned int textAlignment, const float* textMeasureData, const void* caretMeasureData);
	virtual __int64 drawText(Font* font, const float* pos, std::string* text, MC_Color* color, float alpha, unsigned int textAlignment, const TextMeasureData* textMeasureData, const uintptr_t* caretMeasureData);
	virtual void flushText(float timeSinceLastFlush);
};

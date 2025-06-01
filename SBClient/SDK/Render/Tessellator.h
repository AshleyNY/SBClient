#pragma once
#include "../../Utils/MemoryUtils.h"
#include "../../Utils/Math.h"
#include "../../SDK/Signatures.h"

enum class VertextFormat {
	QUAD = 1,
	TRIANGLE_LIST,
	TRIANGLE_STRIP,
	LINE_LIST,
	LINE_STRIP
};

class Tessellator {
public:
	inline void begin(VertextFormat format, int reservedVerticies = 0) {
		using tess_begin_t = void(__fastcall*)(Tessellator*, VertextFormat, int);
		static tess_begin_t tess_begin = reinterpret_cast<tess_begin_t>(findSig(Sigs::tessellator::begin));//1.21.82
		tess_begin(this, format, reservedVerticies);
	}

	inline void vertex(float x, float y, float z) {
		using tess_vertex_t = void(__fastcall*)(Tessellator*, float, float, float);
		static tess_vertex_t tess_vertex = reinterpret_cast<tess_vertex_t>(findSig(Sigs::tessellator::vertex));//1.21.82
		tess_vertex(this, x, y, z);
	}

	inline void color(float r, float g, float b, float a) {
		using tess_color_t = void(__fastcall*)(Tessellator*, float, float, float, float);
		static tess_color_t tess_color = reinterpret_cast<tess_color_t>(findSig(Sigs::tessellator::color));//1.21.82
		tess_color(this, r, g, b, a);
	}
};
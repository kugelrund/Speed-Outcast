#include "strafe_helper_includes.h"
#include "../cgame/cg_local.h"


extern "C" {

void shi_drawFilledRectangle(const float x, const float y,
                             const float w, const float h, const vec4_t color)
{
	CG_FillRect(x, y, w, h, color);
}

} // extern "C"

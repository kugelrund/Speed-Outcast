#include "strafe_helper_includes.h"
#include "../cgame/cg_local.h"
#include "../cgame/cg_media.h"


extern "C" {

void shi_drawFilledRectangle(const float x, const float y,
                             const float w, const float h, const vec4_t color)
{
	CG_FillRect(x, y, w, h, color);
}

void shi_drawString(float x, float y, const char* string, float scale, const vec4_t color)
{
	const float w = cgi_R_Font_StrLenPixels(string, cgs.media.qhFontMedium, scale);
	cgi_R_Font_DrawString(x - w/2, y, string, color, cgs.media.qhFontMedium, -1, scale);
}

} // extern "C"

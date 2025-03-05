#include "SDK_Font.h"

void SDK::Font::Init(wchar_t* FontName, int FontType, int Italic) {
	hDC = wglGetCurrentDC();
	FontList = glGenLists(FONT_LIST_GENERATE_SIZE);

	FontData = CreateFont(
		-1, 0, 0, 0, FontType, Italic, FALSE, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS,
		NONANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, FontName
	);

	Created = true;
}

void SDK::Font::UpdateGlyph(wchar_t*& Text, int TextWordCount) {
	for (int i = 0; i < TextWordCount; i++) {
		if (Text[i] >= FONT_LIST_GENERATE_SIZE)
			return;

		if (GlyphCache.count(Text[i]) == 0) {
			GlyphCache.insert(Text[i]);

			HFONT OldFont = (HFONT)SelectObject(hDC, FontData);
			GLYPHMETRICSFLOAT Glyph;
			wglUseFontOutlinesW(hDC, Text[i], 1, FontList + Text[i], 0.0f, 0.0f, WGL_FONT_POLYGONS, &Glyph);
			TextGlyph.emplace(Text[i], Glyph);
			SelectObject(hDC, OldFont);
		}
	}
}
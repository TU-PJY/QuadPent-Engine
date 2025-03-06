#pragma once
#include "SDK_Header.h"

namespace SDK {
	class Font {
	private:
		HDC                          hDC{};
		HFONT                        FontData{};
		std::unordered_set <wchar_t> GlyphCache{};

	public:
		bool                     InitializedState{};
		unsigned int             FontList{};
		std::unordered_map <wchar_t, GLYPHMETRICSFLOAT> TextGlyph{};

		void Init(wchar_t* FontName, int FontType = FW_DONTCARE, int Italic = FALSE);
		void UpdateGlyph(wchar_t*& Text, int TextWordCount);
	};
}
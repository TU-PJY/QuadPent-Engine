#include "GameResource.h"

// Use LoadT(), LoadClipT(), and LoadSpriteSheetT() to load image files and sprite sheets in this thread.

DWORD WINAPI UserResourceLoader(LPVOID lpParam) {
	// load user resources here


//////////////////////////////////////////////////////////
#ifdef USE_CUSTOM_FONT
	int TotalSize = sizeof(FONT_PATH);
	int ElementSize = sizeof(FONT_PATH[0]);
	int Length = TotalSize / ElementSize;
	for (int i = 0; i < Length; ++i)
		fontUtil.Load(FONT_PATH[i], true);
#endif

	return 0;
}
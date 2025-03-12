#pragma once
#include "QuadPent_Header.h"
#include "QuadPent_EnumValue.h"
#include "QuadPent_ImageTool.h"
#include "QuadPent_SoundTool.h"
#include "QuadPent_Script.h"
#include "QuadPent_Data.h"
#include "QuadPent_FontLoader.h"
#include "QuadPent_Font.h"

namespace ASSET {
	// Declare the sprite resource here. 
	struct QuadPent_IMAGE_RESOURCE {

	};

	// Declare the sound resource here. 
	struct QuadPent_SOUND_RESOURCE {

	};

	// Declare the sound channel resource here.
	struct QuadPent_SOUND_CHANNEL_RESOURCE {

	};

	// Declare the global value here.
	struct QuadPent_GLOBAL_RESOURCE {

	};

	// Data Format list
	// Write your data format here.
	// {  Data Type, Category Name, Data Name, Digit Value, String Value }
	struct QuadPent_FORMAT_RESOURCE {

	};

	// Decalre the file resource here.
	struct QuadPent_FILE_RESOURCE {

	};

	// Declare the font here.
	struct QuadPent_FONT_RESOURCE {

	};

	extern QuadPent_IMAGE_RESOURCE         IMAGE;
	extern QuadPent_SOUND_RESOURCE         SOUND;
	extern QuadPent_SOUND_CHANNEL_RESOURCE CHANNEL;
	extern QuadPent_GLOBAL_RESOURCE        GLOBAL;
	extern QuadPent_FORMAT_RESOURCE        FORMAT;
	extern QuadPent_FILE_RESOURCE          FILE;
	extern QuadPent_FONT_RESOURCE          FONT;

	////////////////////////////////////////////////////////////////////////////////////////
	
	inline void StartUpBehavior() {
		// add behavior here

	}

	// Use LoadImage(), LoadClip(), and LoadSpriteSheet() to load image files and sprite sheets in this thread.
	inline void ResourcePreLoader() {
		// Pre load resources here

	}

	// Use LoadImageT(), LoadClipT(), and LoadSpriteSheetT() to load image files and sprite sheets in this thread.
	inline DWORD WINAPI ImageResourceLoader(LPVOID lpParam) {
		// load image resources here

		return 0;
	}

	inline DWORD WINAPI SoundResourceLoader(LPVOID lpParam) {
		// load sound resources here

		return 0;
	}

	inline DWORD WINAPI FileResourceLoader(LPVOID lpParam) {
		// load file resources here

		return 0;
	}

	// Use LoadT() to load font files in this thread.
	inline DWORD WINAPI FontResourceLoader(LPVOID lpParam) {
		// Load font resources here

		return 0;
	}

	inline void FontResourceInitializer() {
		// initialize font objects here

	}
}
#pragma once
#include "SDK_Header.h"
#include "SDK_EnumValue.h"
#include "SDK_EnumValue.h"
#include "SDK_ImageTool.h"
#include "SDK_SoundTool.h"
#include "SDK_Script.h"
#include "SDK_Data.h"
#include "SDK_FontLoader.h"
#include "SDK_Text.h"

namespace MSDK {
	// Declare the sprite resource here. 
	struct SDK_IMAGE_RESOURCE {

	};

	// Declare the sound resource here. 
	struct SDK_SOUND_RESOURCE {

	};

	// Declare the sound channel resource here.
	struct SDK_SOUND_CHANNEL_RESOURCE {

	};

	// Data Format list
	// Write your data format here.
	// {  Data Type, Category Name, Data Name, Digit Value, String Value }
	struct SDK_FORMAT_RESOURCE {

	};

	// Decalre the file resource here.
	struct SDK_FILE_RESOURCE {

	};

	// Declare the global value here.
	struct SDK_GLOBAL_RESOURCE {

	};

	// Declare the global font name here.
	struct SDK_FONT_NAME_RESOURCE {

	};

	extern SDK_IMAGE_RESOURCE IMAGE;
	extern SDK_SOUND_RESOURCE SOUND;
	extern SDK_SOUND_CHANNEL_RESOURCE CHANNEL;
	extern SDK_GLOBAL_RESOURCE GLOBAL;
	extern SDK_FORMAT_RESOURCE FORMAT;
	extern SDK_FILE_RESOURCE FILE;
	extern SDK_FONT_NAME_RESOURCE FONTNAME;
}

DWORD WINAPI ImageResourceLoader(LPVOID lpParam);
DWORD WINAPI SoundResourceLoader(LPVOID lpParam);
DWORD WINAPI FileResourceLoader(LPVOID lpParam);
DWORD WINAPI FontResourceLoader(LPVOID lpParam);
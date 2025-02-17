#pragma once
#include "SDK_Header.h"
#include "SDK_ImageTool.h"
#include "SDK_SoundTool.h"
#include "SDK_Script.h"
#include "SDK_Data.h"
#include "SDK_FontLoader.h"
#include "SDK_Text.h"

namespace SDK {
	// Declare the sprite resource here. 
	class SDK_IMAGE_RESOURCE {
	public:

	};

	// Declare the sound resource here. 
	class SDK_SOUND_RESOURCE {
	public:

	};

	// Data Format list
	// Write your data format here.
	// {  Data Type, Category Name, Data Name, Digit Value, String Value }
	class SDK_FORMAT_RESOURCE {
	public:

	};

	// Decalre the file resource here.
	class SDK_FILE_RESOURCE {
	public:

	};

	// Declare the global value here.
	class SDK_GLOBAL_RESOURCE {
	public:

	};

	extern SDK_IMAGE_RESOURCE IMAGE;
	extern SDK_SOUND_RESOURCE SOUND;
	extern SDK_GLOBAL_RESOURCE GLOBAL;
	extern SDK_FORMAT_RESOURCE FORMAT;
	extern SDK_FILE_RESOURCE FILE;
}

DWORD WINAPI ImageResourceLoader(LPVOID lpParam);
DWORD WINAPI SoundResourceLoader(LPVOID lpParam);
DWORD WINAPI FileResourceLoader(LPVOID lpParam);
DWORD WINAPI FontResourceLoader(LPVOID lpParam);
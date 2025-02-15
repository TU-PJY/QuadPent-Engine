#pragma once
#include "SDKHeader.h"
#include "SDK_ImageTool.h"
#include "SDK_SoundTool.h"
#include "SDK_Script.h"
#include "SDK_Data.h"
#include "SDK_FontLoader.h"
#include "SDK_Text.h"

// Declare the sprite resource here. 
class SDK_IMAGE {
public:

};

// Declare the sound resource here. 
class SDK_SOUND {
public:

};

// Data Format list
// Write your data format here.
// {  Data Type, Category Name, Data Name, Digit Value, String Value }
class SDK_FORMAT {
public:

};

// Decalre the file resource here.
class SDK_FILE {
public:

};

// Declare the global value here.
class SDK_GLOBAL {
public:

};


namespace SDK {
	extern SDK_IMAGE IMAGE;
	extern SDK_SOUND SOUND;
	extern SDK_GLOBAL GLOBAL;
	extern SDK_FORMAT FORMAT;
	extern SDK_FILE FILE;
}

DWORD WINAPI ImageResourceLoader(LPVOID lpParam);
DWORD WINAPI SoundResourceLoader(LPVOID lpParam);
DWORD WINAPI FileResourceLoader(LPVOID lpParam);
DWORD WINAPI FontResourceLoader(LPVOID lpParam);
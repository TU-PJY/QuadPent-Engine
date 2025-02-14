#pragma once
#include "SDKHeader.h"
#include "ImageUtil.h"
#include "SoundUtil.h"
#include "ScriptUtil.h"
#include "FileUtil.h"
#include "FontUtil.h"
#include "TextUtil.h"

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

extern SDK_IMAGE IMG;
extern SDK_SOUND SND;
extern SDK_GLOBAL GLB;

namespace SDK {
	extern SDK_FORMAT FORMAT;
	extern SDK_FILE FILE;
}

DWORD WINAPI ImageResourceLoader(LPVOID lpParam);
DWORD WINAPI SoundResourceLoader(LPVOID lpParam);
DWORD WINAPI FileResourceLoader(LPVOID lpParam);
DWORD WINAPI FontResourceLoader(LPVOID lpParam);
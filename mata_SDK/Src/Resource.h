#pragma once
#include "SDKHeader.h"
#include "ImageUtil.h"
#include "SoundUtil.h"
#include "ScriptUtil.h"
#include "FileUtil.h"
#include "FontUtil.h"
#include "TextUtil.h"

// Declare the sprite resource here. 
class ___IMAGE {
public:

};

// Declare the sound resource here. 
class ___SOUND {
public:

};

// Data Format list
// Write your data format here.
// {  Data Type, Category Name, Data Name, Digit Value, String Value }
class ___FORMAT {
public:

};

// Decalre the file resource here.
class ___FILE {
public:

};

// Declare the global value here.
class ___GLOBAL {
public:

};

extern ___IMAGE IMG;
extern ___SOUND SND;
extern ___GLOBAL GLOBAL;

namespace SDK {
	extern ___FORMAT FORMAT;
	extern ___FILE FILE;
}

DWORD WINAPI ImageResourceLoader(LPVOID lpParam);
DWORD WINAPI SoundResourceLoader(LPVOID lpParam);
DWORD WINAPI FileResourceLoader(LPVOID lpParam);
DWORD WINAPI FontResourceLoader(LPVOID lpParam);
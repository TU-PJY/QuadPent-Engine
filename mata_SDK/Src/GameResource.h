#pragma once
#include "SDKHeader.h"
#include "ScriptUtil.h"
#include "FileUtil.h"

// Declare the sprite resource or directory here. 
class ImageResource {
public:

};

// Declare the sound resource or directory here. 
class SoundResource {
public:

};

// Data Format list
// Write your data format here.
// {  Data Type, Category Name, Data Name, Digit Value, String Value }
class DataFormatResource {
public:

};

// Declare the global value here.
class GlobalResource {
public:

};

// Load your custom font.
// USE_CUSTON_FONT must be enabled.
//#define USE_CUSTOM_FONT
#ifdef USE_CUSTOM_FONT
constexpr const char* FONT_PATH[]
{

};
#endif

extern ImageResource Img;
extern SoundResource Snd;
extern DataFormatResource Fmt;
extern GlobalResource Glo;

DWORD WINAPI ImageResourceLoader(LPVOID lpParam);
DWORD WINAPI SoundResourceLoader(LPVOID lpParam);
DWORD WINAPI FileResourceLoader(LPVOID lpParam);
DWORD WINAPI FontResourceLoader(LPVOID lpParam);
#pragma once
#include "SDKHeader.h"
#include "ImageUtil.h"
#include "SoundUtil.h"
#include "ScriptUtil.h"
#include "FileUtil.h"
#include "FontUtil.h"
#include "TextUtil.h"

// Declare the ptr of mode class's Start() function here.
// Map the mode start function pointer in Modes.cpp after declaration.
class MODE {
public:
	MODE_PTR Mode1;

};

// Declare the sprite resource here. 
class ImageResource {
public:

};

// Declare the sound resource here. 
class SoundResource {
public:

};

// Data Format list
// Write your data format here.
// {  Data Type, Category Name, Data Name, Digit Value, String Value }
class DataFormatResource {
public:

};

// Decalre the data resource here.
class DataResource {
public:

};

// Declare the global value here.
class GlobalResource {
public:

};

extern ImageResource Img;
extern SoundResource Snd;
extern DataFormatResource Fmt;
extern DataResource Dat;
extern GlobalResource Glb;
extern MODE ModePtr;

DWORD WINAPI ImageResourceLoader(LPVOID lpParam);
DWORD WINAPI SoundResourceLoader(LPVOID lpParam);
DWORD WINAPI DataResourceLoader(LPVOID lpParam);
DWORD WINAPI FontResourceLoader(LPVOID lpParam);
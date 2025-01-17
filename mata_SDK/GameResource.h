#pragma once
#include "EngineHeader.h"

// Declare the sprite resource or directory here. 
class SpriteResource {
public:

};

// Declare the sound resource or directory here. 
class AudioResource {
public:

};

// Data Format list
// Write your data format here.
// {  Data Type, Category Name, Data Name, Digit Value, String Value }
class DataFormatResource {
public:

};

// Declare the global value here.
class GlobalValue {
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

extern SpriteResource Sprite;
extern AudioResource Audio;
extern DataFormatResource Format;
extern GlobalValue Global;
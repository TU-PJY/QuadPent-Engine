#pragma once
#include "EngineHeader.h"

constexpr const char* MGK_LOGO_SOUND_DIRECTORY      = "MGKResource//Sound//MGK_Logo_Sound.wav";
constexpr const char* MGK_LOGO_DIRECTORY		    = "MGKResource//Image//Common//MGK_Logo.png";
constexpr const char* MGK_LOADING_SPINNER_DIRECTORY = "MGKResource//Image//Common//MGK_Loading_Spinner.png";
constexpr const char* FMOD_LOGO_DIRECTORY		    = "MGKResource//Image//Common//FMOD_Logo.png";
constexpr const char* LINE_TEXTURE_DIRECTORY		= "MGKResource//Image//ShapeUtil//Texture_Line.png";
constexpr const char* ROBOTO_FONT_DIRECTORY         = "MGKResource//Font//Roboto-Regular.ttf";

extern Image MGK_LOGO;
extern Image FMOD_LOGO;
extern Image COLOR_TEXTURE;
extern Image LOADING_SPINNER;

extern GLUquadric* GLU_CIRCLE;
extern GLUquadric* GLU_LINE_CIRCLE;

#ifdef USE_SOUND_SYSTEM
extern Sound INTRO_SOUND;
#endif
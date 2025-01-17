#pragma once
#include "EngineHeader.h"

class SystemResource {
public:
	const char* MGK_LOGO_SOUND_DIRECTORY = "SDKResource//Sound//MGK_Logo_Sound.wav";
	const char* MGK_LOGO_DIRECTORY = "SDKResource//Image//Common//MGK_Logo.png";
	const char* MGK_LOADING_SPINNER_DIRECTORY = "SDKResource//Image//Common//MGK_Loading_Spinner.png";
	const char* FMOD_LOGO_DIRECTORY = "SDKResource//Image//Common//FMOD_Logo.png";
	const char* LINE_TEXTURE_DIRECTORY = "SDKResource//Image//ShapeUtil//Texture_Line.png";
	const char* ROBOTO_FONT_DIRECTORY = "SDKResource//Font//Roboto-Regular.ttf";

	Image MGK_LOGO{};
	Image FMOD_LOGO{};
	Image COLOR_TEXTURE{};
	Image LOADING_SPINNER{};

	GLUquadric* GLU_CIRCLE{};
	GLUquadric* GLU_LINE_CIRCLE{};

#ifdef USE_SOUND_SYSTEM
	Sound INTRO_SOUND{};
#endif
};
extern SystemResource SysRes;
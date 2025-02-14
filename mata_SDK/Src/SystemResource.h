#pragma once
#include "SDKHeader.h"

class ___SYSTEM_RES {
public:
	const char* SDK_LOGO_SOUND_DIRECTORY = "SDKResource//Sound//sound-logo.wav";
	const char* SDK_LOGO_IMAGE_DIRECTORY = "SDKResource//Image//Common//logo-SDK.png";
	const char* SDK_ERROR_IMAGE_DIRECTORY = "SDKResource//Image//Common//error.png";
	const char* SDK_LOADING_SPINNER_DIRECTORY = "SDKResource//Image//Common//spinner.png";
	const char* FMOD_LOGO_DIRECTORY = "SDKResource//Image//Common//logo-FMOD.png";
	const char* COLOR_TEXTURE_DIRECTORY = "SDKResource//Image//ShapeUtil//ColorTexture.png";
	const char* SDK_FONT_DIRECTORY = "SDKResource//Font//Roboto-Regular.ttf";

	Image SDK_LOGO{};
	Image SDK_IMAGE_ERROR{};
	Image FMOD_LOGO{};
	Image COLOR_TEXTURE{};
	Image LOADING_SPINNER{};

	GLUquadric* GLU_CIRCLE{};
	GLUquadric* GLU_LINE_CIRCLE{};

	Sound INTRO_SOUND{};
};
extern ___SYSTEM_RES SYSRES;
#pragma once
#include "SDK_Header.h"
namespace MSDK {
	struct SDK_SYSTEM_RESOURCE {
		const char* SDK_LOGO_SOUND_DIRECTORY = "SDKResource//Sound//sound-logo.wav";
		const char* SDK_LOGO_IMAGE_DIRECTORY = "SDKResource//Image//Common//logo-SDK.png";
		const char* MATA_LOGO_IMAGE_DIRECTORY = "SDKResource//Image//Common//logo-mata.png";
		const char* SDK_ERROR_IMAGE_DIRECTORY = "SDKResource//Image//Common//error.png";
		const char* SDK_LOADING_SPINNER_DIRECTORY = "SDKResource//Image//Common//spinner.png";
		const char* FMOD_LOGO_DIRECTORY = "SDKResource//Image//Common//logo-FMOD.png";
		const char* COLOR_TEXTURE_DIRECTORY = "SDKResource//Image//ShapeUtil//ColorTexture.png";
		const char* SDK_FONT_DIRECTORY = "SDKResource//Font//Roboto-Regular.ttf";

		MSDK::Image SDK_LOGO{};
		MSDK::Image MATA_LOGO{};
		MSDK::Image SDK_IMAGE_ERROR{};
		MSDK::Image FMOD_LOGO{};
		MSDK::Image COLOR_TEXTURE{};
		MSDK::Image LOADING_SPINNER{};

		GLUquadric* GLU_CIRCLE{};
		GLUquadric* GLU_LINE_CIRCLE{};

		MSDK::Sound INTRO_SOUND{};
	};

	extern SDK_SYSTEM_RESOURCE SYSRES;
}
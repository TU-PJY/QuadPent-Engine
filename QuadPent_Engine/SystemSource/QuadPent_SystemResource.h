#pragma once
#include "QuadPent_Header.h"
#include "QuadPent_Font.h"

namespace QP {
	struct QuadPent_SYSTEM_RESOURCE {
		const char* QUADPENT_LOGO_IMAGE_DIRECTORY = "SystemComponent//Image//Common//logo-QuadPent.png";
		const char* MATA_LOGO_IMAGE_DIRECTORY	  = "SystemComponent//Image//Common//logo-mata.png";
		const char* FMOD_LOGO_DIRECTORY			  = "SystemComponent//Image//Common//logo-FMOD.png";
		const char* COLOR_TEXTURE_DIRECTORY		  = "SystemComponent//Image//ShapeUtil//ColorTexture.png";
		const char* QUADPENT_LOGO_SOUND_DIRECTORY = "SystemComponent//Sound//sound-logo.wav";
		const char* SYSTEM_FONT_DIRECTORY		  = "SystemComponent//Font//SpaceGrotesk-Regular.ttf";

		Image QUADPENT_LOGO{};
		Image MATA_LOGO{};
		Image FMOD_LOGO{};
		Image COLOR_TEXTURE{};

		GLUquadric* GLU_CIRCLE{};
		GLUquadric* GLU_LINE_CIRCLE{};

		Sound INTRO_SOUND{};

		Font SYSTEM_FONT_REGULAR{};
		Font SYSTEM_FONT_BOLD{};
	};

	extern QuadPent_SYSTEM_RESOURCE SYSRES;
}
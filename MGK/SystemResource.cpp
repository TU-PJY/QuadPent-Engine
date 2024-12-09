#include "SystemResource.h"
#include "ImageUtil.h"
#include "SoundUtil.h"

Image MGK_LOGO;
Image FMOD_LOGO;
Image COLOR_TEXTURE;
Image LOADING_SPINNER;

GLUquadric* GLU_CIRCLE;
GLUquadric* GLU_LINE_CIRCLE;

#ifdef USE_SOUND_SYSTEM
Sound INTRO_SOUND;
#endif
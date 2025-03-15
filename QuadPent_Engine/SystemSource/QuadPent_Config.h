#pragma once
#include "QuadPent_DefaultValue.h"
// QuadPent 8.5
// OpenGL version 4.3 ~ 4.6

//////////////////////////////////// Development options
// Dev mode option
constexpr bool ENABLE_DEV_MODE = false;

// Window size when dev mode enabled
// if ENABLE_DEV_MODE is false, these option values will not be used.
constexpr int DEV_SCREEN_WIDTH = QP::DEFAULT_DEV_SCREEN_WIDTH;
constexpr int DEV_SCREEN_HEIGHT = QP::DEFAULT_DEV_SCREEN_HEIGHT;

// Program termination options when developing a program
// When this option is activated, pressing ESC will immediately terminate the program.
// Used only when ENABLE_DEV_MODE is true.
constexpr bool ENABLE_DEV_EXIT = true;

// console window option
// Used only when ENABLE_DEV_MODE is true.
constexpr bool ENABLE_DEBUG_CONSOLE = true;

// FPS indicator option
// Used only when ENABLE_DEV_MODE is true.
constexpr bool ENABLE_FPS_INDICATOR = true;

// boundbox option
// Used only when ENABLE_DEV_MODE is true.
constexpr bool ENABLE_RENDER_BOUND_BOX = true;

// frustum bound option
// Used only when ENABLE_DEV_MODE is true.
constexpr bool ENABLE_RENDER_FRUSTUM_BOUND = false;

// bound box thickness option
constexpr float BOUND_BOX_THICKNESS = QP::DEFAULT_BOUNDBOX_THICKNESS;



//////////////////////////////////// Engine options
// Window name
constexpr const char* WINDOW_NAME = QP::DEFAULT_WINDOW_NAME;

// Window Icon
// Check the icon allocation ID in resource.h and enter it.
#define QUADPENT_WINDOW_ICON QP::DEFAULT_QUADPENT_WINDOW_ICON

// Application version
// Example:
// APPLICATION_MAJOR_VERSION = 1; APPLICATION_MINOR_VERSION = 0; --> 1.0
constexpr int APPLICATION_MAJOR_VERSION = 1;
constexpr int APPLICATION_MINOR_VERSION = 0;

// Splash option
// If ENABLE_START_UP_SCREEN is false, splash screen will not appeared.
// If ENABLE_DEV_MODE is true, splash screen will not appear.
constexpr bool ENABLE_SPLASH_SCREEN = true;
constexpr int SPLASH_SCREEN_WIDTH = QP::DEFAULT_SPLASH_SCREEN_WIDTH;
constexpr int SPLASH_SCREEN_HEIGHT = QP::DEFAULT_SPLASH_SCREEN_HEIGHT;

// Start Up option
// If ENABLE_DEV_MODE is true, start up screen will not appear.
constexpr bool ENABLE_START_UP_SCREEN = true;

// Fullscreen option
// If ENABLE_DEV_MODE is true, fullscreen switching on start will not work.
constexpr bool ENABLE_FULL_SCREEN = false;

// frame limits option
// If this option is set to SDK::MAX_FRAMERATE, the system will run at maximum framerate.
constexpr int FRAMERATE_LIMIT = QP::MAX_FRAMERATE;

// Set the size of the list to be created from the font object.
constexpr int FONT_LIST_GENERATE_SIZE = QP::DEFAULT_FONT_LIST_GENERATE_SIZE;

// When enabled, the window do not react to ALT key press event.
constexpr bool DISABLE_ALT_EVENT = true;



//////////////////////////////////// Scene options
// scene layer option
// 'EOL' should be typed at last
enum Scene_LAYER
{ LAYER1, LAYER2, LAYER3, EOL };

// Specifies the size of the deletion location buffer
constexpr int COMMAND_LOCATION_BUFFER_SIZE = QP::DEFAULT_COMMAND_LOCATION_BUFFER_SIZE;



//////////////////////////////////// Sound options
// Max sound channel option
constexpr int MAX_CHANNEL_SIZE = QP::DEFAULT_MAX_CHANNEL_SIZE;

// FFT Size option
constexpr int FFT_SIZE = QP::DEFAULT_FFT_SIZE;



//////////////////////////////////// QuadPent_Math options
//When the relevant threshold is reached in SDK_Math::Lerp(), the calculation is stopped immediately to optimize the amount of calculation.
//When set to 0.0, calculation amount optimization is not performed.
constexpr float LERP_THRESHOLD = QP::DEFAULT_LERP_THRESHOLD;



//////////////////////////////////// Image render option
// Set the number of blur filtering times to run when blurring images. The higher the value, the more resources the system consumes.
constexpr int BLUR_EXECUTION = QP::DEFAULT_BLUR_EXECUTION;



//////////////////////////////////// File security options
// Data file security option
// When this option is activated, the data file is saved as an encrypted file.
// When this option is disabled, the data file is saved as an xml file.
constexpr bool USE_FILE_SECURITY = false;

// AES, IV for Data
// Recommend that you make this part private or 
// modify it with the default password before distributing
constexpr unsigned char AES_KEY[16] = "1234567890";
constexpr unsigned char IV_KEY[16]  = "1234567890";
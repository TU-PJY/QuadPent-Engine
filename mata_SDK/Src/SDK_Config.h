#pragma once
#include "SDK_DefaultValue.h"
// mata_SDK 8.0
// OpenGL version 4.3 ~ 4.6

//////////////////////////////////// Window options
// window name
constexpr const char* WINDOW_NAME = SDK::DEFAULT_WINDOW_NAME;

// window icon file path
// Enter the path to the icon file to use in the Windows title bar. 
// If you don't enter anything, it will use the windows default icon.
constexpr const wchar_t* WINDOW_TITLE_BAR_ICON_FILE_PATH = SDK::DEFAULT_ICON_PATH;

// window size
constexpr int WINDOW_WIDTH = SDK::DEFAULT_WINDOW_WIDTH;
constexpr int WINDOW_HEIGHT = SDK::DEFAULT_WINDOW_HEIGHT;

// fullscreen option
constexpr bool FULL_SCREEN_OPTION = false;

// When enabled, the window do not react to ALT key press event.
constexpr bool DISABLE_ALT_EVENT = true;



//////////////////////////////////// SDK options
// Application version
// Example:
// APPLICATION_MAJOR_VERSION = 1; APPLICATION_MINOR_VERSION = 0; --> 1.0
constexpr int APPLICATION_MAJOR_VERSION = 1;
constexpr int APPLICATION_MINOR_VERSION = 0;

// loading screen option
constexpr bool ENABLE_LOADING_SCREEN = true;

// frame limits option
// If this option is set to SDK::MAX_FRAMERATE, the system will run at maximum framerate.
constexpr int FRAME_LIMITS = SDK::MAX_FRAMERATE;

// Set the size of the list to be created from the font object.
constexpr int FONT_LIST_GENERATE_SIZE = SDK::DEFAULT_FONT_LIST_GENERATE_SIZE;



//////////////////////////////////// Development options
// console window option
constexpr bool SHOW_CONSOLE = true;

// FPS indicator option
constexpr bool SHOW_FPS = true;

// show boundbox option
constexpr bool SHOW_BOUND_BOX = true;

// show frustum bound option
constexpr bool SHOW_FRUSTUM_BOUND = false;

// bound box thickness option
constexpr float BOUND_BOX_THICKNESS = SDK::DEFAULT_BOUNDBOX_THICKNESS;

// Program termination options when developing a program
// When this option is activated, pressing ESC will immediately terminate the program.
constexpr bool ENABLE_DEV_EXIT = true;



//////////////////////////////////// Scene Options
// scene layer option
// 'EOL' should be typed at last
enum SDK_LAYER
{ LAYER1, LAYER2, LAYER3, EOL };

// Specifies the size of the deletion location buffer
constexpr int COMMAND_LOCATION_BUFFER_SIZE = SDK::DEFAULT_COMMAND_LOCATION_BUFFER_SIZE;



//////////////////////////////////// Sound Options
// Max sound channel option
constexpr int MAX_CHANNEL_SIZE = SDK::DEFAULT_MAX_CHANNEL_SIZE;

// FFT Size option
constexpr int FFT_SIZE = SDK::DEFAULT_FFT_SIZE;



//////////////////////////////////// SDK_Math Options
//When the relevant threshold is reached in SDK_Math::Lerp(), the calculation is stopped immediately to optimize the amount of calculation.
//When set to 0.0, calculation amount optimization is not performed.
constexpr float LERP_THRESHOLD = SDK::DEFAULT_LERP_THRESHOLD;



//////////////////////////////////// Image render Option
// Set the number of blur filtering times to run when blurring images. The higher the value, the more resources the system consumes.
constexpr int BLUR_EXECUTION = SDK::DEFAULT_BLUR_EXECUTION;



//////////////////////////////////// File security Options
// Data file security option
// When this option is activated, the data file is saved as an encrypted file.
// When this option is disabled, the data file is saved as an xml file.
constexpr bool USE_FILE_SECURITY = false;

// AES, IV for Data
// Recommend that you make this part private or 
// modify it with the default password before distributing
constexpr unsigned char AES_KEY[16] = "1234567890";
constexpr unsigned char IV_KEY[16]  = "1234567890";
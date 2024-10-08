#pragma once

// window name
constexpr const char* WINDOW_NAME = "mata_ENGINE-2";

// Application version
constexpr float APPLICATION_VERSION = 1.0;

// window size
constexpr int WINDOW_WIDTH = 1200;
constexpr int WINDOW_HEIGHT = 800;

// fullscreen option
constexpr bool FULL_SCREEN_OPTION = false;

// console window option
constexpr bool SHOW_CONSOLE = true;

// intro screen option
constexpr bool ENABLE_INTRO_SCREEN = true;

// frame limits option
// If this option is set to 0, the engine will run at maximum framerate.
constexpr int FRAME_LIMITS = 0;

// framework layer option
// 'END' should be typed at last
enum class Layer
{ L1, L2, L3, END };

// framework container reserve option
constexpr int OBJECT_LIST_RESERVE = 500;

// show boundbox option
#define SHOW_BOUND_BOX

// AES, IV for FileUtil
// Recommend that you make this part private or 
// modify it with the default password before distributing
constexpr unsigned char AES_KEY[16]
{ 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 };

constexpr unsigned char IV_KEY[16]
{ 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 };
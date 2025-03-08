#include "SDK_ObjectValue.h"

// Object Value
SDK::Matrix4 SDK::LocalMatrix	         = SDK::Matrix4(1.0);
SDK::Matrix4 SDK::GlobalMatrix           = SDK::Matrix4(1.0);
SDK::Matrix4 SDK::ResultMatrix           = SDK::Matrix4(1.0);

int SDK::LocalFlipFlag  = FLIP_TYPE_NONE;
int SDK::GlobalFlipFlag = FLIP_TYPE_NONE;

SDK::Matrix4 SDK::ViewportPositionMatrix = SDK::Matrix4(1.0);

SDK::Color3  SDK::LocalColorValue;
SDK::Color3  SDK::GlobalColorValue;

float SDK::LocalOpacityValue  = 1.0;
float SDK::LocalBlurValue     = 0.0;

float SDK::GlobalOpacityValue = 1.0;
float SDK::GlobalBlurValue    = 0.0;
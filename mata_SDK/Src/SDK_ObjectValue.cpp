#include "SDK_ObjectValue.h"

// Object Value
SDK::Matrix4 SDK::LocalMatrix	         = SDK::Matrix4(1.0);
SDK::Matrix4 SDK::LocalFlipMatrix        = SDK::Matrix4(1.0);

SDK::Matrix4 SDK::GlobalMatrix           = SDK::Matrix4(1.0);
SDK::Matrix4 SDK::GlobalFlipMatrix       = SDK::Matrix4(1.0);

SDK::Matrix4 SDK::ResultMatrix           = SDK::Matrix4(1.0);

SDK::Matrix4 SDK::ViewportPositionMatrix = SDK::Matrix4(1.0);
SDK::Color3  SDK::LocalColorValue;
SDK::Color3  SDK::GlobalColorValue;

float SDK::LocalOpacityValue;
float SDK::LocalBlurValue;

float SDK::GlobalOpacityValue;
float SDK::GlobalBlurValue;
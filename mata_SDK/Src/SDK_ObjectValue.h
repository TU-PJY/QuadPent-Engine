#pragma once
#include "SDK_Header.h"

namespace SDK {
	extern SDK::Matrix4 LocalMatrix;
	extern SDK::Matrix4 GlobalMatrix;
	extern SDK::Matrix4 ResultMatrix;
	extern int LocalFlipFlag;
	extern int GlobalFlipFlag;

	extern SDK::Matrix4 ViewportPositionMatrix;

	extern SDK::Color3 LocalColorValue, GlobalColorValue;
	extern float LocalOpacityValue, GlobalOpacityValue;
	extern float LocalBlurValue, GlobalBlurValue;
}
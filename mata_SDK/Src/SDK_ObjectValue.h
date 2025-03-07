#pragma once
#include "SDK_Header.h"

namespace SDK {
	extern SDK::Matrix4 LocalMatrix, LocalFlipMatrix;

	extern SDK::Matrix4 GlobalMatrix, GlobalFlipMatrix;
	extern SDK::Matrix4 ResultMatrix;

	extern SDK::Matrix4 ViewportPositionMatrix;

	extern SDK::Color3 LocalColorValue, GlobalColorValue;
	extern float LocalOpacityValue, GlobalOpacityValue;
	extern float LocalBlurValue, GlobalBlurValue;
}
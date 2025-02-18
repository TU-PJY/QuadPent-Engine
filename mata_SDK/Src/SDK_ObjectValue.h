#pragma once
#include "SDK_Header.h"

namespace MSDK {
	extern glm::mat4 MoveMatrix, RotateMatrix, ScaleMatrix, FlipMatrix, ResultMatrix;

	extern glm::mat4 UnitMoveMatrix, UnitRotateMatrix, UnitScaleMatrix;
	extern glm::mat4 UnitFlipMatrix;

	extern glm::mat4 ViewportPositionMatrix;

	extern glm::vec3 ObjectColor;

	extern float UnitBlurValue;
	extern float ObjectBlurValue;

	extern float ObjectOpacityValue;
	extern float UnitOpacityValue;
}
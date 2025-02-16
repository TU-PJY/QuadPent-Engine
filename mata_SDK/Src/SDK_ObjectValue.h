#pragma once
#include "SDK_Header.h"

namespace SDK {
	extern glm::mat4 MoveMatrix, RotateMatrix, ScaleMatrix, FlipMatrix, ResultMatrix;

	extern glm::mat4 UnitMoveMatrix, UnitRotateMatrix, UnitScaleMatrix;
	extern glm::mat4 UnitFlipMatrix;

	extern glm::mat4 ViewportPositionMatrix;

	extern glm::vec3 ObjectColor;

	extern GLfloat UnitBlurValue;
	extern GLfloat ObjectBlurValue;

	extern GLfloat ObjectOpacityValue;
	extern GLfloat UnitOpacityValue;
}
#pragma once
#include "EngineHeader.h"
#include <cmath>

namespace Preset {
	constexpr float MaxPositive = 1.57;
	constexpr float MaxNegative = -1.57;
	constexpr float HalfPositive = 0.52;
	constexpr float HalfNegative = -0.52;
}

class LoopSinAnimation {
private:
	GLfloat Num{};

public:
	GLfloat Update(GLfloat MoveScale, GLfloat Speed, float FrameTime);
	void SetValue(GLfloat Value);
	void Reset();
};

class SinInterpolAnimation{
private:
	GLfloat Num{ Preset::HalfPositive };

public:
	GLfloat Update(GLfloat Value, GLfloat Dest, GLfloat Speed, float FrameTime);
	void Reset();
};

class PopBounceAnimation {
private:
	GLfloat Num1{}, Num2{}, Num3{};

public:
	GLfloat Update(GLfloat SizeDest, GLfloat ShakeValue, GLfloat SizeIncreaseSpeed, GLfloat ShakeSpeed, GLfloat ShakeReduceSpeed, float FrameTime);
	void Reset();
};
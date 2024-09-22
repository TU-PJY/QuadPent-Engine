#pragma once
#include "EngineHeader.h"
#include <cmath>

namespace Preset {
	constexpr float MAX = 3.14;
	constexpr float MIN = -3.14;
	constexpr float HalfPositive = 1.57;
	constexpr float HalfNegative = -1.57;
}

class LoopSinAnimation {
private:
	GLfloat Num{};

public:
	GLfloat Update(GLfloat MoveScale, GLfloat Speed, float FrameTime);
	void SetValue(GLfloat Value);
	void Reset();
};


class PopBounceAnimation {
private:
	GLfloat Num1{}, Num2{}, Num3{};

public:
	GLfloat Update(GLfloat SizeDest, GLfloat ShakeValue, GLfloat SizeIncreaseSpeed, GLfloat ShakeSpeed, GLfloat ShakeReduceSpeed, float FrameTime);
	void Reset();
};


class LerpSinAnimation {
private:
	GLfloat Num{};
	GLfloat Acc{};
	GLfloat Result{};
	bool Increase{ true };

public:
	GLfloat Update(GLfloat From, GLfloat To, GLfloat Speed, GLfloat AccValue, float FrameTime);
	void Reset();
};
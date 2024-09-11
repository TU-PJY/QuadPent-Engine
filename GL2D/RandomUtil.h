#pragma once
#include "EngineHeader.h"
#include <random>

enum class Dist
{Real, Int};

namespace Random {
	GLfloat Gen(Dist DistType, GLfloat Min, GLfloat Max);
}
#pragma once
#include "EngineHeader.h"
#include <random>

enum Dist {
	RANDOM_TYPE_REAL, 
	RANDOM_TYPE_INT
};

namespace Random {
	GLfloat Gen(int DistType, GLfloat Min, GLfloat Max);
}
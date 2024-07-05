#pragma once
#include "GLHeader.h"
#include <random>

enum class Dist
{Real, Int};

class RandomUtil {
private:
	std::random_device RD;

public:
	GLfloat Gen(Dist DistType, GLfloat Min, GLfloat Max);
};
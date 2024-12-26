#pragma once
#include "EngineHeader.h"
#include <random>

class RandomUtil {
private:
	std::random_device RD{};

public:
	GLfloat Gen(int DistType, GLfloat Min, GLfloat Max);
};
extern RandomUtil randomUtil;
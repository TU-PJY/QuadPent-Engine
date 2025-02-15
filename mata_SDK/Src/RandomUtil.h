#pragma once
#include "SDKHeader.h"
#include <random>

class SDK_Random {
private:
	std::random_device RD{};

public:
	GLfloat Gen(int DistType, GLfloat Min, GLfloat Max);
	bool Probability(int Percent);
};

namespace SDK {
	extern SDK_Random Random;
}
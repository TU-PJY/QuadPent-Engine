#pragma once
#include "SDK_Header.h"
#include <random>

namespace SDK {
	class SDK_Random {
	private:
		std::random_device RD{};

	public:
		GLfloat Gen(int DistType, GLfloat Min, GLfloat Max);
		bool Probability(int Percent);
	};

	extern SDK::SDK_Random Random;
}
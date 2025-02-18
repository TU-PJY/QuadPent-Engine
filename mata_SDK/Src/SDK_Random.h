#pragma once
#include "SDK_Header.h"
#include <random>

namespace MSDK {
	class SDK_Random {
	private:
		std::random_device RD{};

	public:
		float Gen(int DistType, float Min, float Max);
		bool Probability(int Percent);
	};

	extern MSDK::SDK_Random Random;
}
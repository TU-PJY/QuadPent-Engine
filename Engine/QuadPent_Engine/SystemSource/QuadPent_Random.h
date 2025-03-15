#pragma once
#include "QuadPent_Header.h"

namespace QP {
	class QuadPent_Random {
	private:
		std::random_device RD{};

	public:
		float Gen(int DistType, float Min, float Max);
		bool Probability(int Percent);
	};

	extern QuadPent_Random Random;
}
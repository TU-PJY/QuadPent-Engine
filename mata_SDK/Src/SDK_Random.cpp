#include "SDK_Random.h"

SDK_Random SDK::Random;

GLfloat SDK_Random::Gen(int DistType, GLfloat Min, GLfloat Max) {
	if (DistType == RANDOM_TYPE_REAL) {
		std::uniform_real_distribution URD{ Min, Max };
		return URD(RD);
	}

	else if (DistType == RANDOM_TYPE_INT) {
		std::uniform_int_distribution UID{ (int)Min, (int)Max };
		return UID(RD);
	}

	return 0.0;
}

bool SDK_Random::Probability(int Percent) {
	int RandNum = Gen(RANDOM_TYPE_INT, 1, 100);
	if (1 <= RandNum && RandNum <= Percent)
		return true;

	return false;
}
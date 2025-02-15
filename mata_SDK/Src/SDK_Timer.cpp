#include "SDK_Timer.h"
#include <cmath>

void SDK::Timer::Update(float FT) {
	if(!StopState)
		Time += FT;
}

void SDK::Timer::Stop() {
	StopState = true;
}

void SDK::Timer::Resume() {
	StopState = false;
}

void SDK::Timer::Reset() {
	Time = 0.0;
}

int SDK::Timer::Sec() {
	return (int)Time;
}

GLfloat SDK::Timer::MiliSec(int DemicalPlace) {
	float TruncateValue = pow(10, DemicalPlace);
	float TruncatedNum = std::floor(Time * TruncateValue) / TruncateValue;
	return TruncatedNum;
}

void SDK::Timer::Interpolate(GLfloat Value) {
	GLfloat OverTime = Time - Value;
	Time = OverTime;
}

bool SDK::Timer::CheckSec(int DestTime, int CheckOption) {
	if (Sec() >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			return true;

		case CHECK_AND_RESET:
			Reset();
			return true;

		case CHECK_AND_INTERPOLATE:
			Interpolate((GLfloat)DestTime);
			return true;

		case CHECK_AND_STOP:
			Stop();
			return true;
		}
	}

	return false;
}

bool SDK::Timer::CheckMiliSec(GLfloat DestTime, int DemicalPlace, int CheckOption) {
	if (MiliSec(DemicalPlace) >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			return true;

		case CHECK_AND_RESET:
			Reset();
			return true;

		case CHECK_AND_INTERPOLATE:
			Interpolate(DestTime);
			return true;

		case CHECK_AND_STOP:
			Stop();
			return true;
		}
	}
	return false;
}


void SDK::Metronome::SetBPM(int BPM) {
	BeatInterval = 60.0 / (GLdouble)BPM;
}

void SDK::Metronome::Update(float FT) {
	if (!StopState) {
		Time += FT;

		if (Time >= BeatInterval) {
			GLdouble OverTime = Time - BeatInterval;
			Time = OverTime;
			BeatState = true;
		}

		else
			BeatState = false;
	}
}

void SDK::Metronome::Stop() {
	StopState = true;
	BeatState = false;
}

void SDK::Metronome::Resume() {
	StopState = false;
}

void SDK::Metronome::Reset() {
	Time = 0.0;
}

bool SDK::Metronome::IsBeat() {
	return BeatState;
}
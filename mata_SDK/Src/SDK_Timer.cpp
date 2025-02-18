#include "SDK_Timer.h"
#include <cmath>

void MSDK::Timer::Update(float FrameTime) {
	if(!StopState)
		Time += FrameTime;
}

void MSDK::Timer::Stop() {
	StopState = true;
}

void MSDK::Timer::Resume() {
	StopState = false;
}

void MSDK::Timer::Reset() {
	Time = 0.0;
}

float MSDK::Timer::GetCurrentTime() {
	return Time;
}

int MSDK::Timer::Sec() {
	return (int)Time;
}

float MSDK::Timer::MiliSec(int DemicalPlace) {
	float TruncateValue = pow(10, DemicalPlace);
	float TruncatedNum = std::floor(Time * TruncateValue) / TruncateValue;
	return TruncatedNum;
}

void MSDK::Timer::Interpolate(float Value) {
	float OverTime = Time - Value;
	Time = OverTime;
}

bool MSDK::Timer::UpdateAndCheckSec(int DestTime, int CheckOption, float FrameTime) {
	Time += FrameTime;

	if (Sec() >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			return true;

		case CHECK_AND_RESET:
			Reset();
			return true;

		case CHECK_AND_INTERPOLATE:
			Interpolate((float)DestTime);
			return true;

		case CHECK_AND_STOP:
			Stop();
			return true;
		}
	}

	return false;
}

bool MSDK::Timer::UpdateAndCheckMiliSec(int DestTime, int DemicalPlace, int CheckOption, float FrameTime) {
	Time += FrameTime;

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

bool MSDK::Timer::CheckSec(int DestTime, int CheckOption) {
	if (Sec() >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			return true;

		case CHECK_AND_RESET:
			Reset();
			return true;

		case CHECK_AND_INTERPOLATE:
			Interpolate((float)DestTime);
			return true;

		case CHECK_AND_STOP:
			Stop();
			return true;
		}
	}

	return false;
}

bool MSDK::Timer::CheckMiliSec(float DestTime, int DemicalPlace, int CheckOption) {
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


void MSDK::Metronome::SetBPM(int BPM) {
	BeatInterval = 60.0 / (float)BPM;
}

void MSDK::Metronome::Update(float FrameTime) {
	if (!StopState) {
		Time += FrameTime;

		if (Time >= BeatInterval) {
			float OverTime = Time - BeatInterval;
			Time = OverTime;
			BeatState = true;
		}

		else
			BeatState = false;
	}
}

void MSDK::Metronome::Stop() {
	StopState = true;
	BeatState = false;
}

void MSDK::Metronome::Resume() {
	StopState = false;
}

void MSDK::Metronome::Reset() {
	Time = 0.0;
}

bool MSDK::Metronome::IsBeat() {
	return BeatState;
}
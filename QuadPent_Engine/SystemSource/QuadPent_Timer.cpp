#include "QuadPent_Timer.h"

void QP::Timer::Update(float FrameTime) {
	if(!StopState)
		Time += FrameTime;
}

void QP::Timer::Stop() {
	StopState = true;
}

void QP::Timer::Resume() {
	StopState = false;
}

void QP::Timer::Reset() {
	Time = 0.0;
}

float QP::Timer::GetCurrentTime() {
	return Time;
}

int QP::Timer::Sec() {
	return (int)Time;
}

float QP::Timer::MiliSec(int DemicalPlace) {
	float TruncateValue = pow(10, DemicalPlace);
	float TruncatedNum = std::floor(Time * TruncateValue) / TruncateValue;
	return TruncatedNum;
}

void QP::Timer::Interpolate(float Value) {
	float OverTime = Time - Value;
	Time = OverTime;
}

bool QP::Timer::UpdateAndCheckSec(int DestTime, int CheckOption, float FrameTime) {
	Update(FrameTime);

	if (Sec() >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			break;

		case CHECK_AND_RESET:
			Reset();
			break;

		case CHECK_AND_INTERPOLATE:
			Interpolate((float)DestTime);
			break;

		case CHECK_AND_STOP:
			Stop();
			break;
		}

		return true;
	}

	return false;
}

bool QP::Timer::UpdateAndCheckMiliSec(float DestTime, int DemicalPlace, int CheckOption, float FrameTime) {
	Update(FrameTime);

	if (MiliSec(DemicalPlace) >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			break;

		case CHECK_AND_RESET:
			Reset();
			break;

		case CHECK_AND_INTERPOLATE:
			Interpolate(DestTime);
			break;

		case CHECK_AND_STOP:
			Stop();
			break;
		}

		return true;
	}

	return false;
}

bool QP::Timer::CheckSec(int DestTime, int CheckOption) {
	if (Sec() >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			break;

		case CHECK_AND_RESET:
			Reset();
			break;

		case CHECK_AND_INTERPOLATE:
			Interpolate((float)DestTime);
			break;

		case CHECK_AND_STOP:
			Stop();
			break;
		}

		return true;
	}

	return false;
}

bool QP::Timer::CheckMiliSec(float DestTime, int DemicalPlace, int CheckOption) {
	if (MiliSec(DemicalPlace) >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			break;

		case CHECK_AND_RESET:
			Reset();
			break;

		case CHECK_AND_INTERPOLATE:
			Interpolate(DestTime);
			break;

		case CHECK_AND_STOP:
			Stop();
			break;
		}

		return true;
	}
	return false;
}


void QP::Metronome::SetBPM(int BPM) {
	BeatInterval = 60.0 / (float)BPM;
}

void QP::Metronome::Update(float FrameTime) {
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

void QP::Metronome::Stop() {
	StopState = true;
	BeatState = false;
}

void QP::Metronome::Resume() {
	StopState = false;
}

void QP::Metronome::Reset() {
	Time = 0.0;
}

bool QP::Metronome::IsBeat() {
	return BeatState;
}
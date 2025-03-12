#include "QuadPent_EXTool.h"

QP::QuadPent_EXTool QP::EXTool;

void QP::QuadPent_EXTool::SwitchBool(bool& Value) {
	if (!Value)
		Value = true;
	else
		Value = false;
}

void QP::QuadPent_EXTool::SwitchValue(float& TargetValue, float Value1, float Value2) {
	if (TargetValue == Value1)
		TargetValue = Value2;
	else
		TargetValue = Value1;
}

void QP::QuadPent_EXTool::SwitchValue(int& TargetValue, int Value1, int Value2) {
	if (TargetValue == Value1)
		TargetValue = Value2;
	else
		TargetValue = Value1;
}

void QP::QuadPent_EXTool::ClampValue(float& Value, float Dest, int Type) {
	switch (Type) {
	case CLAMP_GREATER:
		if (Value > Dest) Value = Dest;
		break;

	case CLAMP_LESS:
		if (Value < Dest) Value = Dest;
		break;
	}
}

void QP::QuadPent_EXTool::ClampValue(int& Value, int Dest, int Type) {
	switch (Type) {
	case CLAMP_GREATER:
		if (Value > Dest) Value = Dest;
		break;

	case CLAMP_LESS:
		if (Value < Dest) Value = Dest;
		break;
	}
}

void QP::QuadPent_EXTool::ClampValue(int& Value, int Min, int Max, int Type) {
	switch (Type) {
	case CLAMP_FIXED:
		if (Value < Min)
			Value = Min;
		else if (Value > Max)
			Value = Max;
		break;

	case CLAMP_RETURN:
		if (Value < Min)
			Value = Max;
		else if (Value > Max)
			Value = Min;
	}
}

void QP::QuadPent_EXTool::ClampValue(float& Value, float Min, float Max, int Type) {
	switch (Type) {
	case CLAMP_FIXED:
		if (Value < Min)
			Value = Min;
		else if (Value > Max)
			Value = Max;
		break;

	case CLAMP_RETURN:
		if (Value < Min)
			Value = Max;
		else if (Value > Max)
			Value = Min;
	}
}

bool QP::QuadPent_EXTool::CheckClampValue(float& Value, float Dest, int Type) {
	ClampValue(Value, Dest, Type);
	if (Value == Dest)
		return true;

	return false;
}

bool QP::QuadPent_EXTool::CheckClampValue(int& Value, int Dest, int Type) {
	ClampValue(Value, Dest, Type);
	if (Value == Dest)
		return true;

	return false;
}

void QP::QuadPent_EXTool::InputBoolSwitch(int SwitchingOption, QP::KeyEvent& Event, WPARAM Key, bool& DestBoolValue) {
	if ((Event.Type == WM_KEYDOWN || Event.Type == WM_KEYUP) && Event.Key != Key)
		return;

	if (SwitchingOption == TRUE_KEY_DOWN) {
		if (Event.Type == WM_KEYDOWN)
			DestBoolValue = true;

		else if (Event.Type == WM_KEYUP)
			DestBoolValue = false;
	}

	else if (SwitchingOption == FALSE_KEY_DOWN) {
		if (Event.Type == WM_KEYDOWN)
			DestBoolValue = false;

		else if (Event.Type == WM_KEYUP)
			DestBoolValue = true;
	}
}
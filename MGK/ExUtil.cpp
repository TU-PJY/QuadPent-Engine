#include "ExUtil.h"

void EX_Util::SwitchBool(bool& Value) {
	if (!Value)
		Value = true;
	else
		Value = false;
}

void EX_Util::SwitchValue(float& TargetValue, float Value1, float Value2) {
	if (TargetValue == Value1)
		TargetValue = Value2;
	else
		TargetValue = Value1;
}

void EX_Util::SwitchValue(int& TargetValue, int Value1, int Value2) {
	if (TargetValue == Value1)
		TargetValue = Value2;
	else
		TargetValue = Value1;
}

void EX_Util::ClampValue(float& Value, float Dest, int Type) {
	switch (Type) {
	case CLAMP_GREATER:
		if (Value >= Dest) Value = Dest;
		break;

	case CLAMP_LESS:
		if (Value <= Dest) Value = Dest;
		break;
	}
}

void EX_Util::ClampValue(int& Value, int Dest, int Type) {
	switch (Type) {
	case CLAMP_GREATER:
		if (Value >= Dest) Value = Dest;
		break;

	case CLAMP_LESS:
		if (Value <= Dest) Value = Dest;
		break;
	}
}

bool EX_Util::CheckClampValue(float& Value, float Dest, int Type) {
	EX_Util::ClampValue(Value, Dest, Type);
	if (Value == Dest)
		return true;

	return false;
}

bool EX_Util::CheckClampValue(int& Value, int Dest, int Type) {
	EX_Util::ClampValue(Value, Dest, Type);
	if (Value == Dest)
		return true;

	return false;
}
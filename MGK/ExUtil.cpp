#include "ExUtil.h"

void EX::SwitchBool(bool& Value) {
	if (!Value)
		Value = true;
	else
		Value = false;
}

void EX::SwitchValue(float& TargetValue, float Value1, float Value2) {
	if (TargetValue == Value1)
		TargetValue = Value2;
	else
		TargetValue = Value1;
}

void EX::SwitchValue(int& TargetValue, int Value1, int Value2) {
	if (TargetValue == Value1)
		TargetValue = Value2;
	else
		TargetValue = Value1;
}

void EX::ClampValue(float& Value, float Dest, int Type) {
	switch (Type) {
	case CLAMP_GREATER:
		if (Value > Dest) Value = Dest;
		break;

	case CLAMP_LESS:
		if (Value < Dest) Value = Dest;
		break;
	}
}

void EX::ClampValue(int& Value, int Dest, int Type) {
	switch (Type) {
	case CLAMP_GREATER:
		if (Value > Dest) Value = Dest;
		break;

	case CLAMP_LESS:
		if (Value < Dest) Value = Dest;
		break;
	}
}
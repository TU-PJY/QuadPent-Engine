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

void EX::ClampValue(float& Value, float Dest, ClampType Type) {
	if ((Type == ClampType::Greater && Value > Dest) || (Type == ClampType::Less && Value < Dest))
		Value = Dest;
}
#pragma once
#include "EngineHeader.h"

class EX_Util {
public:
	void SwitchBool(bool& Value);
	void SwitchValue(float& TargetValue, float Value1, float Value2);
	void SwitchValue(int& TargetValue, int Value1, int Value2);
	void ClampValue(float& Value, float Dest, int Type);
	void ClampValue(int& Value, int Dest, int Type);
	bool CheckClampValue(float& Value, float Dest, int Type);
	bool CheckClampValue(int& Value, int Dest, int Type);
};
extern EX_Util EX;
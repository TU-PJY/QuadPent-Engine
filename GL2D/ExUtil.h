#pragma once

enum class ClampType
{ Greater, Less };

namespace EX {
	void SwitchBool(bool& Value);
	void SwitchValue(float& TargetValue, float Value1, float Value2);
	void ClampValue(float& Value, float Dest, ClampType Type);
 }
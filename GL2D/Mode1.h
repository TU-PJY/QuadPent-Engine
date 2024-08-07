#pragma once
#include "MouseUtil.h"
#include <string>

namespace Mode1 {
	enum class KeyType
	{NormalKey, SpecialKey};

	enum class KeyState
	{Down, Up};

	void SetController();
	std::string GameMode1();

	void ProcessKeyboard(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey);
	void ProcessMouse(int button, int state, int x, int y);
	void ProcessMouseWheel(int button, int Wheel, int x, int y);
	void KeyDown(unsigned char KEY, int x, int y);
	void KeyUp(unsigned char KEY, int x, int y);
	void SpecialKeyUp(int KEY, int x, int y);
	void SpecialKeyDown(int KEY, int x, int y);
	void MouseMotion(int x, int y);
	void MousePassiveMotion(int x, int y);
}
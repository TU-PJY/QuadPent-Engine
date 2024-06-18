#pragma once
#include "MouseUtil.h"

namespace Mode_1 {
	inline void MouseMotion(int x, int y) {
		mouse.ConvertPosition(x, y);
	}

	inline void MousePassiveMotion(int x, int y) {
		mouse.ConvertPosition(x, y);
	}

	inline void KeyDown(unsigned char KEY, int x, int y) {
		switch (KEY) {
		case 27:
			exit(1);
			break;
		}
	}

	inline void KeyUp(unsigned char KEY, int x, int y) {

	}
}
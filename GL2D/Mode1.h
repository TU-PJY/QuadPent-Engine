#pragma once
#include "MouseUtil.h"
#include "FWL.h"
#include <string>

std::string Mode1();


inline void KeyDown(unsigned char KEY, int x, int y) {

}

inline void KeyUp(unsigned char KEY, int x, int y) {

}

inline void SpecialKeyUp(int KEY, int x, int y) {

}

inline void SpecialKeyDown(int KEY, int x, int y) {

}

inline void MouseButton(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		switch (state) {
		case GLUT_DOWN:
			mouse.LButtonDown = true;
			break;

		case GLUT_UP:
			mouse.LButtonDown = false;
			break;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		switch (state) {
		case GLUT_DOWN:
			mouse.RButtonDown = true;
			break;

		case GLUT_UP:
			mouse.RButtonDown = false;
			break;
		}
		break;
	}
}

inline void MouseWheel(int button, int dir, int x, int y) {
	if (dir > 0) {
	}

	else if (dir < 0) {
	}
}

inline void MouseMotion(int x, int y) {
	mouse.ConvertPosition(x, y);
}

inline void MousePassiveMotion(int x, int y) {
	mouse.ConvertPosition(x, y);
}
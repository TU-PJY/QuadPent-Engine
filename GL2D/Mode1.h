#pragma once
#include "FrameWork.h"
#include "MouseUtil.h"
#include <string>

namespace Mode_1 {
	inline void MouseButton(int button, int state, int x, int y) {
		switch (button) {
		case GLUT_LEFT_BUTTON:
			switch (state) {
			case GLUT_DOWN:
				break;

			case GLUT_UP:
				break;
			}

			break;

		case GLUT_RIGHT_BUTTON:
			switch (state) {
			case GLUT_DOWN:
				break;

			case GLUT_UP:
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

	inline void KeyDown(unsigned char KEY, int x, int y) {

	}

	inline void KeyUp(unsigned char KEY, int x, int y) {

	}

	inline void SpecialKeyUp(int KEY, int x, int y) {

	}

	inline void SpecialKeyDown(int KEY, int x, int y) {

	}
}

inline std::string Mode1() {
	glutMotionFunc(Mode_1::MouseMotion);
	glutPassiveMotionFunc(Mode_1::MousePassiveMotion);
	glutKeyboardFunc(Mode_1::KeyDown);
	glutKeyboardUpFunc(Mode_1::KeyUp);
	glutMouseWheelFunc(Mode_1::MouseWheel);
	glutSpecialFunc(Mode_1::SpecialKeyDown);
	glutSpecialUpFunc(Mode_1::SpecialKeyUp);

	return __func__;
}
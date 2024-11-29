#include "GameMode1.h"
#include "Scene.h"
#include "MouseUtil.h"

// You can get the mode file template from ModeTemplate.txt

class test : public GameObject {
public:
	AABB aabb;
	AABB aabb2;
	OOBB oobb, oobb2;
	BoundingCircle bc, bc2;
	GLfloat Rotation{};

	void UpdateFunc(float FT) {
		bc.Update(mouse.x, mouse.y, 0.5);
		bc2.Update(0.0, 0.0, 0.5);
		//bc2.CheckCollisionPoint(mouse.x, mouse.y);
		bc2.CheckCollision(bc);
	}

	void RenderFunc() {
		InitRenderState();
		Transform::Scale(ScaleMatrix, 0.0, 0.0);
		Transform::Move(TranslateMatrix, mouse.x, mouse.y);
		Render(COLOR_TEXTURE);
		bc.Render();
		bc2.Render();
	}
};

void GameMode1::Start() {
	Framework::SetBackColor(0.5, 0.5, 0.5);

	scene.AddObject(new test, "test", LAYER1);

	scene.RegisterController(Controller, MODE_TYPE_DEFAULT);
	scene.RegisterDestructor(Destructor);
	scene.RegisterModeName("GameMode1");
}

void GameMode1::Destructor() {
}

void GameMode1::ProcessKeyboard(int State, unsigned char NormalKey, int SpecialKey) {
	if (State == NORMAL_KEY_DOWN) {
		switch (NormalKey) {
		case NK_ESCAPE:
			Framework::Exit();
			break;
		}
	}
}

void GameMode1::ProcessMouseButton(int State) {
}

void GameMode1::ProcessMouseWheel(int State) {
}



void GameMode1::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_DOWN, KEY, NULL);
}

void GameMode1::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_UP, KEY, NULL);
}

void GameMode1::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_DOWN, NULL, KEY);
}

void GameMode1::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_UP, NULL, KEY);
}

void GameMode1::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void GameMode1::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void GameMode1::MouseWheel(int Button, int Wheel, int X, int Y) {
	if (Wheel > 0)
		ProcessMouseWheel(WHEEL_UP);
	else if (Wheel < 0)
		ProcessMouseWheel(WHEEL_DOWN);
}

void GameMode1::MouseButton(int Button, int State, int X, int Y) {
	if (State == GLUT_DOWN) {
		if (Button == GLUT_LEFT_BUTTON)
			ProcessMouseButton(LEFT_BUTTON_DOWN);
		else if (Button == GLUT_RIGHT_BUTTON)
			ProcessMouseButton(RIGHT_BUTTON_DOWN);
		else if (Button == GLUT_MIDDLE_BUTTON)
			ProcessMouseButton(MIDDLE_BUTTON_DOWN);
	}
	else if (State == GLUT_UP) {
		if (Button == GLUT_LEFT_BUTTON)
			ProcessMouseButton(LEFT_BUTTON_UP);
		else if (Button == GLUT_RIGHT_BUTTON)
			ProcessMouseButton(RIGHT_BUTTON_UP);
		else if (Button == GLUT_MIDDLE_BUTTON)
			ProcessMouseButton(MIDDLE_BUTTON_UP);
	}
}

void GameMode1::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(MouseWheel);
	glutMouseFunc(MouseButton);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}
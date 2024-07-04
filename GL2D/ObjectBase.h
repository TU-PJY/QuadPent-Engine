#pragma once
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <string>

class OBJ_BASE {
public:
	bool DeleteDesc{};
	bool FloatingObjectDesc{};
	std::string ObjectTag{};

	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };
	glm::vec3 ObjectColor{ glm::vec3(0.0, 0.0, 0.0) };
	GLfloat AlphaValue{ 1.0f };

	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{}, TextAlphaLocation{};

	void Translate(GLfloat MoveX, GLfloat MoveY);
	void Rotate(GLfloat Radians);
	void Scale(GLfloat ScaleX, GLfloat ScaleY);
	void RotateSpot(GLfloat Radians);
	void RotateAxis(GLfloat Radians, GLfloat AxisX, GLfloat AxisY);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetAlpha(GLfloat Value);
	void InitTransform();

	virtual void Update(float FT) {}
	virtual void Render() {}
	virtual void InputKey(unsigned char KEY, int S_KEY, bool KeyDown, bool SpecialKey = false) {}
	virtual void InputMouse(int button, int state, int x, int y) {}
	virtual void InputScroll(int button, int Wheel, int x, int y) {}
	void ProcessTransform();

	virtual void ResetControlState() {}
};

// dummy object for avoiding iterator error
class FWM_DUMMY : public OBJ_BASE {};
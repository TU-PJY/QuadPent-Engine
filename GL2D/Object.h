#pragma once
#include "Shader.h"
#include "RenderModeUtil.h"
#include <string>

class OBJ {
public:
	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };
	GLfloat AlphaValue{ 1.0f };
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{}, TextAlphaLocation{};

	bool NoUpdateAtPause{};
	std::string Tag{};
	Layer ObjectLayer{};

	void Move(GLfloat MoveX, GLfloat MoveY) {
		TranslateMatrix = translate(TranslateMatrix, glm::vec3(MoveX, MoveY, 0.0));
	}

	void Rotate(GLfloat Radians) {
		RotateMatrix = rotate(RotateMatrix, glm::radians(Radians), glm::vec3(0.0, 0.0, 1.0));
	}

	void Scale(GLfloat ScaleX, GLfloat ScaleY) {
		ScaleMatrix = scale(ScaleMatrix, glm::vec3(ScaleX, ScaleY, 0.0));
	}

	void RotateSpot(GLfloat Radians) {
		TranslateMatrix = rotate(TranslateMatrix, glm::radians(Radians), glm::vec3(0.0, 0.0, 1.0));
	}

	void MoveAspect(GLfloat MoveX, GLfloat MoveY) {
		TranslateMatrix = translate(TranslateMatrix, glm::vec3(MoveX * ASPECT, MoveY, 0.0));
	}

	void SetAlpha(GLfloat Value) {
		AlphaValue = Value;
	}

	void InitTransform() {
		TranslateMatrix = glm::mat4(1.0f);
		RotateMatrix = glm::mat4(1.0f);
		ScaleMatrix = glm::mat4(1.0f);
	}

	void ProcessTransform() {
		renderMode.SetImageMode();

		TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
		glUniform1f(TransparencyLocation, AlphaValue);

		ModelLocation = glGetUniformLocation(ImageShader, "model");
		glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(RotateMatrix * TranslateMatrix * ScaleMatrix));
	}

	virtual void Update(float FT) {}
	virtual void Render() {}
};
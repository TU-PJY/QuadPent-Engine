#include "ObjectBase.h"
#include "RenderModeUtil.h"


void OBJ::Move(GLfloat MoveX, GLfloat MoveY) {
	TranslateMatrix = translate(TranslateMatrix, glm::vec3(MoveX, MoveY, 0.0));
}

void OBJ::Rotate(GLfloat Radians) {
	RotateMatrix = rotate(RotateMatrix, glm::radians(Radians), glm::vec3(0.0, 0.0, 1.0));
}

void OBJ::Scale(GLfloat ScaleX, GLfloat ScaleY) {
	ScaleMatrix = scale(ScaleMatrix, glm::vec3(ScaleX, ScaleY, 0.0));
}

void OBJ::RotateSpot(GLfloat Radians) {
	TranslateMatrix = rotate(TranslateMatrix, glm::radians(Radians), glm::vec3(0.0, 0.0, 1.0));
}

void OBJ::MoveAspect(GLfloat MoveX, GLfloat MoveY) {
	TranslateMatrix = translate(TranslateMatrix, glm::vec3(MoveX * ASPECT, MoveY, 0.0));
}

void OBJ::SetAlpha(GLfloat Value) {
	AlphaValue = Value;
}

void OBJ::InitTransform() {
	TranslateMatrix = glm::mat4(1.0f);
	RotateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
}

void OBJ::ProcessTransform() {
	renderMode.SetImageMode();

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, AlphaValue);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(RotateMatrix * TranslateMatrix * ScaleMatrix));
}
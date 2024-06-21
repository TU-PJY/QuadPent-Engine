#include "ObjectBase.h"
#include "RenderModeUtil.h"


void OBJ::Translate(GLfloat X, GLfloat Y) {
	TranslateMatrix = translate(TranslateMatrix, glm::vec3(X, Y, 0.0));
}

void OBJ::Rotate(GLfloat Radians) {
	RotateMatrix = rotate(RotateMatrix, glm::radians(Radians), glm::vec3(0.0, 0.0, 1.0));
}

void OBJ::Scale(GLfloat X, GLfloat Y) {
	ScaleMatrix = scale(ScaleMatrix, glm::vec3(X, Y, 0.0));
}

void OBJ::RotateSpot(GLfloat Radians) {
	TranslateMatrix = rotate(TranslateMatrix, glm::radians(Radians), glm::vec3(0.0, 0.0, 1.0));
}

void OBJ::RotateAxis(GLfloat Radians, GLfloat AxisX, GLfloat AxisY) {
	TranslateMatrix = translate(TranslateMatrix, glm::vec3(AxisX, AxisY, 0.0));
	TranslateMatrix = rotate(TranslateMatrix, glm::radians(Radians), glm::vec3(0.0, 0.0, 1.0));
	TranslateMatrix = translate(TranslateMatrix, glm::vec3(-AxisX, -AxisY, 0.0));
}

void OBJ::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	ObjectColor.r = R;
	ObjectColor.g = G;
	ObjectColor.b = B;
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

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, ObjectColor.r, ObjectColor.g, ObjectColor.b);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(RotateMatrix * TranslateMatrix * ScaleMatrix));
}
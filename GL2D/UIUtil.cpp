#include "UIUtil.h"
#include "ImageUtil.h"
#include "RenderModeUtil.h"

void Button::Init(std::string ImageName) {
	imageUtil.SetImage(Image, ImageName);
	aabb.Init();
}

void Button::Move(GLfloat X, GLfloat Y) {
	TranslateMatrix = translate(TranslateMatrix, glm::vec3(X, Y, 0.0));
}

void Button::Rotate(GLfloat RotationValue) {
	TranslateMatrix = rotate(TranslateMatrix, glm::radians(RotationValue), glm::vec3(0.0, 0.0, 1.0));
}

void Button::Scale(GLfloat X, GLfloat Y) {
	TranslateMatrix = scale(TranslateMatrix, glm::vec3(X, Y, 1.0));
}

void Button::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	ButtonColor.r = R;
	ButtonColor.g = G;
	ButtonColor.b = B;
}

void Button::SetTransparency(GLfloat Value) {
	Transparency = Value;
}

void Button::SetCollisionArea(GLfloat X, GLfloat Y, GLfloat SizeH, GLfloat SizeV) {
	PositionX = X;
	PositionY = Y;
	SizeX = SizeH;
	SizeY = SizeV;
}

bool Button::CheckCursorCollision(GLfloat X, GLfloat Y) {
	if (aabb.CheckCollisionPoint(X, Y))
		return true;
	return false;
}

void Button::InitTransform() {
	TranslateMatrix = glm::mat4(1.0f);
	Transparency = 1.0f;
}

void Button::Render(GLfloat Width, GLfloat Height, ButtonFlip FlipOption) {
	if (Width != 1.0 || Height != 1.0)
		TranslateMatrix = scale(TranslateMatrix, glm::vec3(Width / Height, 1.0, 1.0));

	switch (FlipOption) {
	case ButtonFlip::Horizontal:
		TranslateMatrix = rotate(TranslateMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		break;
	case ButtonFlip::Vertical:
		TranslateMatrix = rotate(TranslateMatrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
		break;
	}

	ProcessTransform();
	imageUtil.Render(Image);
	if(SizeX > 0.0 && SizeY > 0.0)
		aabb.Update(PositionX, PositionY, SizeX, SizeY);
}

void Button::ProcessTransform() {
	renderMode.SetImageMode();

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, Transparency);

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, ButtonColor.r, ButtonColor.g, ButtonColor.b);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix));
}



void Cursor::Init(std::string ImageName){
	imageUtil.SetImage(Image, ImageName);
}

void Cursor::SetColor(GLfloat R, GLfloat G, GLfloat B){
	CursorColor.r = R;
	CursorColor.g = G;
	CursorColor.b = B;
}

void Cursor::SetTransparency(GLfloat Value) {
	Transparency = Value;
}

void Cursor::Move(GLfloat X, GLfloat Y) {
	TranslateMatrix = translate(TranslateMatrix, glm::vec3(X, Y, 0.0));
}

void Cursor::Rotate(GLfloat RotationValue) {
	TranslateMatrix = rotate(TranslateMatrix, glm::radians(RotationValue), glm::vec3(0.0, 0.0, 1.0));
}

void Cursor::Scale(GLfloat SizeH, GLfloat SizeV) {
	TranslateMatrix = scale(TranslateMatrix, glm::vec3(SizeH, SizeV, 1.0));
}

void Cursor::InitTransform() {
	TranslateMatrix = glm::mat4(1.0f);
	Transparency = 1.0f;
}

void Cursor::Render(GLfloat Width, GLfloat Height, CursorFlip FlipOption) {
	if (Width != 1.0f || Height != 1.0f)
		TranslateMatrix = scale(TranslateMatrix, glm::vec3(Width / Height, 1.0, 1.0));

	switch (FlipOption) {
	case CursorFlip::Horizontal:
		TranslateMatrix = rotate(TranslateMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		break;
	case CursorFlip::Vertical:
		TranslateMatrix = rotate(TranslateMatrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
		break;
	}

	ProcessTransform();
	imageUtil.Render(Image);
}

void Cursor::ProcessTransform() {
	renderMode.SetImageMode();

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, Transparency);

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, CursorColor.r, CursorColor.g, CursorColor.b);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix));
}
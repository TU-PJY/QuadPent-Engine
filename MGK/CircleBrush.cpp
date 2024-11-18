#include "CircleBrush.h"
#include "CameraUtil.h"
#include "TransformUtil.h"
#include "SystemResource.h"

CircleBrush::CircleBrush(bool Flag) {
	CamInheritanceCommand = Flag;
}

void CircleBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void CircleBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void CircleBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void CircleBrush::Draw(GLfloat X, GLfloat Y, GLfloat Diameter, GLfloat Transparency) {
	Transform::Identity(TranslateMatrix);
	TransparencyValue = 1.0f;

	Transform::Move(TranslateMatrix, X, Y);
	TransparencyValue = Transparency;
	Radius = Diameter * 0.5;

	Render();
}

void CircleBrush::Render() {
	if (!CamInheritanceCommand)
		camera.SetCamera(RenderType);

	glUseProgram(ImageShader);
	camera.PrepareRender(SHADER_TYPE_IMAGE);

	glUniform1f(ImageTransparencyLocation, TransparencyValue);
	glUniform3f(ImageColorLocation, Color.r, Color.g, Color.b);
	glUniform1i(BoolBlurLocation, 0);
	glUniformMatrix4fv(ImageModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix));

	glBindTexture(GL_TEXTURE_2D, 0);
	gluDisk(GLU_CIRCLE, 0.0, Radius, 80, 1);
}



LineCircleBrush::LineCircleBrush(bool Flag) {
	CamInheritanceCommand = Flag;
}

void LineCircleBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void LineCircleBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void LineCircleBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void LineCircleBrush::Draw(GLfloat X, GLfloat Y, GLfloat Diameter, GLfloat Width, GLfloat Transparency) {
	Transform::Identity(TranslateMatrix);
	TransparencyValue = 1.0f;

	Transform::Move(TranslateMatrix, X, Y);
	TransparencyValue = Transparency;
	Radius = Diameter * 0.5;
	WidthValue = Width;

	Render();
}

void LineCircleBrush::Render() {
	if (!CamInheritanceCommand)
		camera.SetCamera(RenderType);

	glUseProgram(ImageShader);
	camera.PrepareRender(SHADER_TYPE_IMAGE);

	glUniform1f(ImageTransparencyLocation, TransparencyValue);
	glUniform3f(ImageColorLocation, Color.r, Color.g, Color.b);
	glUniform1i(BoolBlurLocation, 0);
	glUniformMatrix4fv(ImageModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix));

	glBindTexture(GL_TEXTURE_2D, 0);
	gluDisk(GLU_LINE_CIRCLE, Radius, Radius + WidthValue, 80, 1);
}
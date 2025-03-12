#include "QuadPent_CircleBrush.h"
#include "QuadPent_Camera.h"
#include "QuadPent_Transform.h"
#include "QuadPent_SystemResource.h"

glm::mat4 CircleMatrix;

QP::CircleBrush::CircleBrush(bool CamInheritanceFlag) {
	CamInheritanceCommand = CamInheritanceFlag;
}

void QP::CircleBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void QP::CircleBrush::SetColor(float R, float G, float B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void QP::CircleBrush::SetColor(QP::Color3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void QP::CircleBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (float)R;
	Color.g = (1.0f / 255.0f) * (float)G;
	Color.b = (1.0f / 255.0f) * (float)B;
}

void QP::CircleBrush::Render(float X, float Y, float Diameter, float OpacityValue) {
	Transform.Identity(CircleMatrix);
	Opacity = 1.0f;

	CircleMatrix = translate(CircleMatrix, QP::Vector3(X, Y, 0.0));
	Opacity = OpacityValue;
	Radius = Diameter * 0.5;

	ProcessTransform();
}

void QP::CircleBrush::ProcessTransform() {
	if (!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(CircleMatrix));

	gluDisk(QP::SYSRES.GLU_CIRCLE, 0.0, Radius, 80, 1);
}



QP::LineCircleBrush::LineCircleBrush(bool CamInheritanceFlag, bool StaticWidthFlag) {
	CamInheritanceCommand = CamInheritanceFlag;
	StaticWidthCommand = StaticWidthFlag;
}

void QP::LineCircleBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void QP::LineCircleBrush::SetColor(float R, float G, float B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void QP::LineCircleBrush::SetColor(QP::Color3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void QP::LineCircleBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (float)R;
	Color.g = (1.0f / 255.0f) * (float)G;
	Color.b = (1.0f / 255.0f) * (float)B;
}

void QP::LineCircleBrush::Draw(float X, float Y, float Diameter, float Thickness, float OpacityValue) {
	Transform.Identity(CircleMatrix);
	Opacity = 1.0f;

	CircleMatrix = translate(CircleMatrix, QP::Vector3(X, Y, 0.0));
	Opacity = OpacityValue;
	Radius = Diameter * 0.5;

	if(RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		WidthValue = Thickness / Camera.Zoom;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		WidthValue = Thickness;

	Render();
}

void QP::LineCircleBrush::Render() {
	if (!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(CircleMatrix));

	gluDisk(QP::SYSRES.GLU_LINE_CIRCLE, Radius - WidthValue * 0.5, Radius + WidthValue * 0.5, 80, 1);
}
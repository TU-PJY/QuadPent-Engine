#include "RectBrush.h"
#include "SDK_ImageTool.h"
#include "SDK_Camera.h"
#include "SDK_Transform.h"
#include "SystemResource.h"
#include "SDK_ComputeTool.h"
#include <cmath>

SDK::LineRectBrush::LineRectBrush(bool CameraInheritanceFlag, bool StaticWidthFlag) {
	CamInheritanceCommand = CameraInheritanceFlag;
	StaticWidthCommand = StaticWidthFlag;
}

void SDK::LineRectBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void SDK::LineRectBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void SDK::LineRectBrush::SetColor(glm::vec3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void SDK::LineRectBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void SDK::LineRectBrush::Draw(GLfloat X, GLfloat Y, GLfloat SizeX, GLfloat SizeY, GLfloat Width, GLfloat RotationValue, GLfloat OpacityValue) {
	Opacity = OpacityValue;
	GLfloat DrawWidth{};
	if (RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		DrawWidth = Width / Camera.ZoomValue;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		DrawWidth = Width;

	DrawLine(X, Y, 0.0, SizeY / 2.0, SizeX + DrawWidth, DrawWidth, RotationValue);
	DrawLine(X, Y, 0.0, -SizeY / 2.0, SizeX + DrawWidth, DrawWidth, RotationValue);
	DrawLine(X, Y, -SizeX / 2.0, 0.0, DrawWidth, SizeY + DrawWidth, RotationValue);
	DrawLine(X, Y, SizeX / 2.0, 0.0, DrawWidth, SizeY + DrawWidth, RotationValue);
}

void SDK::LineRectBrush::DrawLine(GLfloat X, GLfloat Y, GLfloat OffsetX, GLfloat OffsetY, GLfloat Width, GLfloat Height, GLfloat RotationValue) {
	Transform.Identity(ShapeMatrix);

	Transform.Move(ShapeMatrix, X, Y);
	Transform.Rotate(ShapeMatrix, RotationValue);
	Transform.Move(ShapeMatrix, OffsetX, OffsetY);
	Transform.Scale(ShapeMatrix, Width, Height);
	Render();
}

void SDK::LineRectBrush::Render() {
	if (!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ShapeMatrix));

	SDK::ImageTool.RenderRaw();
}



SDK::RectBrush::RectBrush(bool Flag) {
	CamInheritanceCommand = Flag;
}

void SDK::RectBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void SDK::RectBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void SDK::RectBrush::SetColor(glm::vec3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void SDK::RectBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void SDK::RectBrush::Draw(GLfloat X, GLfloat Y, GLfloat SizeX, GLfloat SizeY, GLfloat RotationValue, GLfloat OpacityValue) {
	Transform.Identity(ShapeMatrix);
	Opacity = OpacityValue;

	Transform.Move(ShapeMatrix, X, Y);
	Transform.Rotate(ShapeMatrix, RotationValue);
	Transform.Scale(ShapeMatrix, SizeX, SizeY);

	Render();
}

void SDK::RectBrush::Render() {
	if(!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ShapeMatrix));

	SDK::ImageTool.RenderRaw();
}

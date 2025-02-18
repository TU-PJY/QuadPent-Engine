#include "SDK_RectBrush.h"
#include "SDK_ImageTool.h"
#include "SDK_Camera.h"
#include "SDK_Transform.h"
#include "SDK_SystemResource.h"
#include "SDK_ComputeTool.h"
#include <cmath>

glm::mat4 RectMatrix;

MSDK::LineRectBrush::LineRectBrush(bool CameraInheritanceFlag, bool StaticWidthFlag) {
	CamInheritanceCommand = CameraInheritanceFlag;
	StaticWidthCommand = StaticWidthFlag;
}

void MSDK::LineRectBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void MSDK::LineRectBrush::SetColor(float R, float G, float B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void MSDK::LineRectBrush::SetColor(glm::vec3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void MSDK::LineRectBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (float)R;
	Color.g = (1.0f / 255.0f) * (float)G;
	Color.b = (1.0f / 255.0f) * (float)B;
}

void MSDK::LineRectBrush::Draw(float X, float Y, float SizeX, float SizeY, float Width, float RotationValue, float OpacityValue) {
	Opacity = OpacityValue;
	float DrawWidth{};
	if (RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		DrawWidth = Width / Camera.Zoom;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		DrawWidth = Width;

	DrawLine(X, Y, 0.0, SizeY / 2.0, SizeX + DrawWidth, DrawWidth, RotationValue);
	DrawLine(X, Y, 0.0, -SizeY / 2.0, SizeX + DrawWidth, DrawWidth, RotationValue);
	DrawLine(X, Y, -SizeX / 2.0, 0.0, DrawWidth, SizeY + DrawWidth, RotationValue);
	DrawLine(X, Y, SizeX / 2.0, 0.0, DrawWidth, SizeY + DrawWidth, RotationValue);
}

void MSDK::LineRectBrush::DrawLine(float X, float Y, float OffsetX, float OffsetY, float Width, float Height, float RotationValue) {
	Transform.Identity(RectMatrix);

	Transform.Move(RectMatrix, X, Y);
	Transform.Rotate(RectMatrix, RotationValue);
	Transform.Move(RectMatrix, OffsetX, OffsetY);
	Transform.Scale(RectMatrix, Width, Height);
	Render();
}

void MSDK::LineRectBrush::Render() {
	if (!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(RectMatrix));

	MSDK::ImageTool.RenderRaw();
}



MSDK::RectBrush::RectBrush(bool Flag) {
	CamInheritanceCommand = Flag;
}

void MSDK::RectBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void MSDK::RectBrush::SetColor(float R, float G, float B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void MSDK::RectBrush::SetColor(glm::vec3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void MSDK::RectBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (float)R;
	Color.g = (1.0f / 255.0f) * (float)G;
	Color.b = (1.0f / 255.0f) * (float)B;
}

void MSDK::RectBrush::Draw(float X, float Y, float SizeX, float SizeY, float RotationValue, float OpacityValue) {
	Transform.Identity(RectMatrix);
	Opacity = OpacityValue;

	Transform.Move(RectMatrix, X, Y);
	Transform.Rotate(RectMatrix, RotationValue);
	Transform.Scale(RectMatrix, SizeX, SizeY);

	Render();
}

void MSDK::RectBrush::Render() {
	if(!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(RectMatrix));

	MSDK::ImageTool.RenderRaw();
}

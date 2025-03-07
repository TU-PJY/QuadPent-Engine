#include "SDK_RectBrush.h"
#include "SDK_ImageTool.h"
#include "SDK_Camera.h"
#include "SDK_Transform.h"
#include "SDK_SystemResource.h"
#include "SDK_ComputeTool.h"

glm::mat4 RectMatrix;

SDK::LineRectBrush::LineRectBrush(bool CameraInheritanceFlag, bool StaticWidthFlag) {
	CamInheritanceCommand = CameraInheritanceFlag;
	StaticWidthCommand = StaticWidthFlag;
}

void SDK::LineRectBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void SDK::LineRectBrush::SetColor(float R, float G, float B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void SDK::LineRectBrush::SetColor(SDK::Color3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void SDK::LineRectBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (float)R;
	Color.g = (1.0f / 255.0f) * (float)G;
	Color.b = (1.0f / 255.0f) * (float)B;
}

void SDK::LineRectBrush::Draw(float X, float Y, float SizeX, float SizeY, float Thickness, float Degree, float OpacityValue) {
	Opacity = OpacityValue;
	float DrawWidth{};
	if (RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		DrawWidth = Thickness / Camera.Zoom;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		DrawWidth = Thickness;

	DrawLine(X, Y, 0.0, SizeY / 2.0, SizeX + DrawWidth, DrawWidth, Degree);
	DrawLine(X, Y, 0.0, -SizeY / 2.0, SizeX + DrawWidth, DrawWidth, Degree);
	DrawLine(X, Y, -SizeX / 2.0, 0.0, DrawWidth, SizeY + DrawWidth, Degree);
	DrawLine(X, Y, SizeX / 2.0, 0.0, DrawWidth, SizeY + DrawWidth, Degree);
}

void SDK::LineRectBrush::DrawLine(float X, float Y, float OffsetX, float OffsetY, float Width, float Height, float Degree) {
	Transform.Identity(RectMatrix);

	RectMatrix = translate(RectMatrix, SDK::Vector3(X, Y, 0.0));
	RectMatrix = rotate(RectMatrix, glm::radians(-Degree), SDK::Vector3(0.0, 0.0, 1.0));
	RectMatrix = translate(RectMatrix, SDK::Vector3(OffsetX, OffsetY, 0.0));
	RectMatrix = scale(RectMatrix, SDK::Vector3(Width, Height, 0.0));
	Render();
}

void SDK::LineRectBrush::Render() {
	if (!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(RectMatrix));

	SDK::ImageTool.RenderRaw();
}



SDK::RectBrush::RectBrush(bool Flag) {
	CamInheritanceCommand = Flag;
}

void SDK::RectBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void SDK::RectBrush::SetColor(float R, float G, float B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void SDK::RectBrush::SetColor(SDK::Color3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void SDK::RectBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (float)R;
	Color.g = (1.0f / 255.0f) * (float)G;
	Color.b = (1.0f / 255.0f) * (float)B;
}

void SDK::RectBrush::Draw(float X, float Y, float SizeX, float SizeY, float Degree, float OpacityValue) {
	Transform.Identity(RectMatrix);
	Opacity = OpacityValue;

	RectMatrix = translate(RectMatrix, SDK::Vector3(X, Y, 0.0));
	RectMatrix = rotate(RectMatrix, glm::radians(-Degree), SDK::Vector3(0.0, 0.0, 1.0));
	RectMatrix = scale(RectMatrix, SDK::Vector3(SizeX, SizeY, 1.0));

	Render();
}

void SDK::RectBrush::Render() {
	if(!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(RectMatrix));

	SDK::ImageTool.RenderRaw();
}

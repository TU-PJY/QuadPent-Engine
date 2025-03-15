#include "QuadPent_RectBrush.h"
#include "QuadPent_ImageTool.h"
#include "QuadPent_Camera.h"
#include "QuadPent_Transform.h"
#include "QuadPent_SystemResource.h"

QP::Matrix4 RectMatrix;

QP::LineRectBrush::LineRectBrush(bool CameraInheritanceFlag, bool StaticWidthFlag) {
	CamInheritanceCommand = CameraInheritanceFlag;
	StaticWidthCommand = StaticWidthFlag;
}

void QP::LineRectBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void QP::LineRectBrush::SetColor(float R, float G, float B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void QP::LineRectBrush::SetColor(QP::Color3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void QP::LineRectBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (float)R;
	Color.g = (1.0f / 255.0f) * (float)G;
	Color.b = (1.0f / 255.0f) * (float)B;
}

void QP::LineRectBrush::Render(float X, float Y, float SizeX, float SizeY, float Thickness, float Degree, float OpacityValue) {
	Opacity = OpacityValue;
	float DrawWidth{};
	if (RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		DrawWidth = Thickness / Camera.Zoom;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		DrawWidth = Thickness;

	RenderLine(X, Y, 0.0, SizeY / 2.0, SizeX + DrawWidth, DrawWidth, Degree);
	RenderLine(X, Y, 0.0, -SizeY / 2.0, SizeX + DrawWidth, DrawWidth, Degree);
	RenderLine(X, Y, -SizeX / 2.0, 0.0, DrawWidth, SizeY + DrawWidth, Degree);
	RenderLine(X, Y, SizeX / 2.0, 0.0, DrawWidth, SizeY + DrawWidth, Degree);
}

void QP::LineRectBrush::RenderLine(float X, float Y, float OffsetX, float OffsetY, float Width, float Height, float Degree) {
	Transform.Identity(RectMatrix);

	RectMatrix = translate(RectMatrix, Vector3(X, Y, 0.0));
	RectMatrix = rotate(RectMatrix, glm::radians(-Degree), Vector3(0.0, 0.0, 1.0));
	RectMatrix = translate(RectMatrix, Vector3(OffsetX, OffsetY, 0.0));
	RectMatrix = scale(RectMatrix, Vector3(Width, Height, 0.0));
	ProcessTransform();
}

void QP::LineRectBrush::ProcessTransform() {
	if (!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(RectMatrix));

	ImageTool.RenderRaw();
}



QP::RectBrush::RectBrush(bool Flag) {
	CamInheritanceCommand = Flag;
}

void QP::RectBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void QP::RectBrush::SetColor(float R, float G, float B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void QP::RectBrush::SetColor(QP::Color3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void QP::RectBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (float)R;
	Color.g = (1.0f / 255.0f) * (float)G;
	Color.b = (1.0f / 255.0f) * (float)B;
}

void QP::RectBrush::Render(float X, float Y, float SizeX, float SizeY, float Degree, float OpacityValue) {
	Transform.Identity(RectMatrix);
	Opacity = OpacityValue;

	RectMatrix = translate(RectMatrix, Vector3(X, Y, 0.0));
	RectMatrix = rotate(RectMatrix, glm::radians(-Degree), QP::Vector3(0.0, 0.0, 1.0));
	RectMatrix = scale(RectMatrix, Vector3(SizeX, SizeY, 1.0));

	ProcessTransform();
}

void QP::RectBrush::ProcessTransform() {
	if(!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(RectMatrix));

	ImageTool.RenderRaw();
}

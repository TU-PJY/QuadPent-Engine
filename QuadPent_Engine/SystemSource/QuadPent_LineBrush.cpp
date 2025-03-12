#include "QuadPent_LineBrush.h"
#include "QuadPent_Transform.h"
#include "QuadPent_Math.h"
#include "QuadPent_ImageTool.h"
#include "QuadPent_Shader.h"
#include "QuadPent_Camera.h"
#include "QuadPent_SystemResource.h"

glm::mat4 LineMatrix;

QP::LineBrush::LineBrush(bool StaticWidthFlag) {
	StaticWidthCommand = StaticWidthFlag;
}

void QP::LineBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void QP::LineBrush::SetColor(float R, float G, float B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void QP::LineBrush::SetColor(QP::Color3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void QP::LineBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (float)R;
	Color.g = (1.0f / 255.0f) * (float)G;
	Color.b = (1.0f / 255.0f) * (float)B;
}

void QP::LineBrush::SetLineType(int LineTypeOpt) {
	if (LineTypeOpt > LINE_TYPE_ROUND)
		return;

	LineType = LineTypeOpt;
}

void QP::LineBrush::Draw(float X1, float Y1, float X2, float Y2, float Thickness, float OpacityValue) {
	Transform.Identity(LineMatrix);
	Opacity = OpacityValue;

	Length = Math.ComputeDistance(X1, Y1, X2, Y2);
	Rotation = Math.ComputeRadians(X1, Y1, X2, Y2);

	LineMatrix = translate(LineMatrix, QP::Vector3(X1, Y1, 0.0));
	LineMatrix = rotate(LineMatrix, -Rotation, QP::Vector3(0.0, 0.0, 1.0));
	LineMatrix = translate(LineMatrix, QP::Vector3(Length / 2.0, 0.0, 0.0));

	float DrawWidth{};
	if (RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		DrawWidth = Thickness / Camera.Zoom;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		DrawWidth = Thickness;

	if (LineType == LINE_TYPE_RECT)
		LineMatrix = scale(LineMatrix, QP::Vector3(Length + DrawWidth, DrawWidth, 1.0));
	else if (LineType == LINE_TYPE_ROUND) 
		LineMatrix = scale(LineMatrix, QP::Vector3(Length, DrawWidth, 1.0));

	Render();

	if (LineType == LINE_TYPE_ROUND)
		DrawCircle(X1, Y1, X2, Y2, DrawWidth);
}

void QP::LineBrush::Render() {
	Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(LineMatrix));

	QP::ImageTool.RenderRaw();
}

void QP::LineBrush::DrawCircle(float X1, float Y1, float X2, float Y2, float Thickness) {
	Transform.Identity(LineMatrix);
	LineMatrix = translate(LineMatrix, QP::Vector3(X1, Y1, 0.0));
	RenderCircle(Thickness);

	Transform.Identity(LineMatrix);
	LineMatrix = translate(LineMatrix, QP::Vector3(X2, Y2, 0.0));
	RenderCircle(Thickness);
}

void QP::LineBrush::RenderCircle(float Thickness) {
	Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(LineMatrix));

	gluDisk(QP::SYSRES.GLU_CIRCLE, 0.0, Thickness * 0.5, 80, 1);
}

#include "LineBrush.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "ImageUtil.h"
#include "ShaderUtil.h"
#include "CameraUtil.h"
#include "SystemResource.h"
#include "ComputeUtil.h"

SDK::LineBrush::LineBrush(bool StaticWidthFlag) {
	StaticWidthCommand = StaticWidthFlag;
}

void SDK::LineBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void SDK::LineBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void SDK::LineBrush::SetColor(glm::vec3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void SDK::LineBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void SDK::LineBrush::SetLineType(int LineTypeOpt) {
	if (LineTypeOpt > LINE_TYPE_ROUND)
		return;

	LineType = LineTypeOpt;
}

void SDK::LineBrush::Draw(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2, GLfloat Width, GLfloat OpacityValue) {
	Transform.Identity(ShapeMatrix);
	Opacity = OpacityValue;

	Length = Math.ComputeDistance(X1, Y1, X2, Y2);
	Rotation = Math.ComputeRadians(X1, Y1, X2, Y2);

	Transform.Move(ShapeMatrix, X1, Y1);
	Transform.RotateRadians(ShapeMatrix, Rotation);
	Transform.Move(ShapeMatrix, Length / 2.0, 0.0);

	GLfloat DrawWidth{};
	if (RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		DrawWidth = Width / Camera.ZoomValue;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		DrawWidth = Width;

	if (LineType == LINE_TYPE_RECT)
		Transform.Scale(ShapeMatrix, Length + DrawWidth, DrawWidth);
	else if (LineType == LINE_TYPE_ROUND) 
		Transform.Scale(ShapeMatrix, Length, DrawWidth);

	Render();

	if (LineType == LINE_TYPE_ROUND)
		DrawCircle(X1, Y1, X2, Y2, DrawWidth);
}

void SDK::LineBrush::DrawLineX(GLfloat X1, GLfloat X2, GLfloat Y, GLfloat Width, GLfloat OpacityValue) {
	Transform.Identity(ShapeMatrix);
	Opacity = OpacityValue;

	GLfloat DrawWidth{};
	if (RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		DrawWidth = Width / Camera.ZoomValue;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		DrawWidth = Width;

	Transform.Move(ShapeMatrix, (X1 + X2) / 2.0, Y);
	if (LineType == LINE_TYPE_RECT)
		Transform.Scale(ShapeMatrix, fabs(X1 - X2) + DrawWidth, DrawWidth);
	else if (LineType == LINE_TYPE_ROUND) 
		Transform.Scale(ShapeMatrix, fabs(X1 - X2), DrawWidth);

	Render();

	if (LineType == LINE_TYPE_ROUND)
		DrawCircle(X1, Y, X2, Y, DrawWidth);
}

void SDK::LineBrush::DrawLineY(GLfloat Y1, GLfloat Y2, GLfloat X, GLfloat Width, GLfloat OpacityValue) {
	Transform.Identity(ShapeMatrix);
	Opacity = OpacityValue;

	GLfloat DrawWidth{};
	if (RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		DrawWidth = Width / Camera.ZoomValue;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		DrawWidth = Width;

	Transform.Move(ShapeMatrix, X, (Y1 + Y2) / 2.0);
	if (LineType == LINE_TYPE_RECT)
		Transform.Scale(ShapeMatrix, DrawWidth, fabs(Y1 - Y2) + DrawWidth);
	else if (LineType == LINE_TYPE_ROUND)
		Transform.Scale(ShapeMatrix, fabs(Y1 - Y2), DrawWidth);

	Render();

	if (LineType == LINE_TYPE_ROUND)
		DrawCircle(X, Y1, X, Y2, DrawWidth);
}

void SDK::LineBrush::Render() {
	Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ShapeMatrix));

	SDK::ImageTool.RenderRaw();
}

void SDK::LineBrush::DrawCircle(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2, GLfloat Width) {
	Transform.Identity(ShapeMatrix);
	Transform.Move(ShapeMatrix, X1, Y1);
	RenderCircle(Width);

	Transform.Identity(ShapeMatrix);
	Transform.Move(ShapeMatrix, X2, Y2);
	RenderCircle(Width);
}

void SDK::LineBrush::RenderCircle(GLfloat Width) {
	Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ShapeMatrix));

	gluDisk(SDK::SYSRES.GLU_CIRCLE, 0.0, Width * 0.5, 80, 1);
}

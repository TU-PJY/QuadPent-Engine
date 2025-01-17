#include "LineBrush.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "ImageUtil.h"
#include "ShaderUtil.h"
#include "CameraUtil.h"
#include "SystemResource.h"
#include "ComputeUtil.h"

void LineBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void LineBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void LineBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void LineBrush::SetLineType(int LineTypeOpt) {
	if (LineTypeOpt > LINE_TYPE_ROUND)
		return;

	LineType = LineTypeOpt;
}

void LineBrush::Draw(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2, GLfloat Width, GLfloat OpacityValue) {
	transform.Identity(TranslateMatrix);
	transform.Identity(ScaleMatrix);
	Opacity = OpacityValue;

	Length = mathUtil.ComputeDistance(X1, Y1, X2, Y2);
	Rotation = mathUtil.ComputeRadians(X1, Y1, X2, Y2);

	transform.Move(TranslateMatrix, X1, Y1);
	transform.RotateRadians(TranslateMatrix, Rotation);
	transform.Move(TranslateMatrix, Length / 2.0, 0.0);

	if (LineType == LINE_TYPE_RECT)
		transform.Scale(ScaleMatrix, Length + Width, Width);
	else if (LineType == LINE_TYPE_ROUND) 
		transform.Scale(ScaleMatrix, Length, Width);

	Render();

	if (LineType == LINE_TYPE_ROUND)
		DrawCircle(X1, Y1, X2, Y2, Width);
}

void LineBrush::DrawLineX(GLfloat X1, GLfloat X2, GLfloat Y, GLfloat Width, GLfloat OpacityValue) {
	transform.Identity(TranslateMatrix);
	transform.Identity(ScaleMatrix);
	Opacity = OpacityValue;

	transform.Move(TranslateMatrix, (X1 + X2) / 2.0, Y);
	if (LineType == LINE_TYPE_RECT)
		transform.Scale(ScaleMatrix, fabs(X1 - X2) + Width, Width);
	else if (LineType == LINE_TYPE_ROUND) 
		transform.Scale(ScaleMatrix, fabs(X1 - X2), Width);

	Render();

	if (LineType == LINE_TYPE_ROUND)
		DrawCircle(X1, Y, X2, Y, Width);
}

void LineBrush::DrawLineY(GLfloat Y1, GLfloat Y2, GLfloat X, GLfloat Width, GLfloat OpacityValue) {
	transform.Identity(TranslateMatrix);
	transform.Identity(ScaleMatrix);
	Opacity = OpacityValue;

	transform.Move(TranslateMatrix, X, (Y1 + Y2) / 2.0);
	if (LineType == LINE_TYPE_RECT)
		transform.Scale(ScaleMatrix, Width, fabs(Y1 - Y2) + Width);
	else if (LineType == LINE_TYPE_ROUND)
		transform.Scale(ScaleMatrix, fabs(Y1 - Y2), Width);

	Render();

	if (LineType == LINE_TYPE_ROUND)
		DrawCircle(X, Y1, X, Y2, Width);
}

void LineBrush::Render() {
	camera.SetCamera(RenderType);

	computeUtil.ComputeMatrix(ResultMatrix, TranslateMatrix, ScaleMatrix);

	glUseProgram(SHAPE_SHADER);
	camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ResultMatrix));

	imageUtil.RenderRaw();
}

void LineBrush::DrawCircle(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2, GLfloat Width) {
	transform.Identity(TranslateMatrix);
	transform.Move(TranslateMatrix, X1, Y1);
	RenderCircle(Width);

	transform.Identity(TranslateMatrix);
	transform.Move(TranslateMatrix, X2, Y2);
	RenderCircle(Width);
}

void LineBrush::RenderCircle(GLfloat Width) {
	camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(TranslateMatrix));

	gluDisk(SysRes.GLU_CIRCLE, 0.0, Width * 0.5, 80, 1);
}

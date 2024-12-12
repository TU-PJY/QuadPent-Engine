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

void LineBrush::Draw(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2, GLfloat Width, GLfloat Transparency) {
	Transform::Identity(TranslateMatrix);
	Transform::Identity(ScaleMatrix);
	TransparencyValue = Transparency;

	Length = Math::ComputeDistance(X1, Y1, X2, Y2);
	Rotation = Math::ComputeRadians(X1, Y1, X2, Y2);

	Transform::Move(TranslateMatrix, X1, Y1);
	Transform::RotateRadians(TranslateMatrix, Rotation);
	Transform::Move(TranslateMatrix, Length / 2.0, 0.0);
	Transform::Scale(ScaleMatrix, Length + Width, Width);

	Render();
}

void LineBrush::DrawLineX(GLfloat X1, GLfloat X2, GLfloat Y, GLfloat Width, GLfloat Transparency) {
	Transform::Identity(TranslateMatrix);
	Transform::Identity(ScaleMatrix);
	TransparencyValue = Transparency;

	Transform::Move(TranslateMatrix, (X1 + X2) / 2.0, Y);
	Transform::Scale(ScaleMatrix, fabs(X1 - X2) + Width, Width);

	Render();
}

void LineBrush::DrawLineY(GLfloat Y1, GLfloat Y2, GLfloat X, GLfloat Width, GLfloat Transparency) {
	Transform::Identity(TranslateMatrix);
	Transform::Identity(ScaleMatrix);
	TransparencyValue = Transparency;

	Transform::Move(TranslateMatrix, X, (Y1 + Y2) / 2.0);
	Transform::Scale(ScaleMatrix, Width, fabs(Y1 - Y2) + Width);

	Render();
}

void LineBrush::Render() {
	camera.SetCamera(RenderType);

	Compt::ComputeMatrix(ResultMatrix, TranslateMatrix, ScaleMatrix);

	glUseProgram(SHAPE_SHADER);
	camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_ALPHA_LOCATION, TransparencyValue);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ResultMatrix));

	imageUtil.RenderRaw();
}

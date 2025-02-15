#include "CircleBrush.h"
#include "SDK_Camera.h"
#include "SDK_Transform.h"
#include "SystemResource.h"

SDK::CircleBrush::CircleBrush(bool CamInheritanceFlag) {
	CamInheritanceCommand = CamInheritanceFlag;
}

void SDK::CircleBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void SDK::CircleBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void SDK::CircleBrush::SetColor(glm::vec3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void SDK::CircleBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void SDK::CircleBrush::Draw(GLfloat X, GLfloat Y, GLfloat Diameter, GLfloat OpacityValue) {
	Transform.Identity(ShapeMatrix);
	Opacity = 1.0f;

	Transform.Move(ShapeMatrix, X, Y);
	Opacity = OpacityValue;
	Radius = Diameter * 0.5;

	Render();
}

void SDK::CircleBrush::Render() {
	if (!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ShapeMatrix));

	gluDisk(SDK::SYSRES.GLU_CIRCLE, 0.0, Radius, 80, 1);
}



SDK::LineCircleBrush::LineCircleBrush(bool CamInheritanceFlag, bool StaticWidthFlag) {
	CamInheritanceCommand = CamInheritanceFlag;
	StaticWidthCommand = StaticWidthFlag;
}

void SDK::LineCircleBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void SDK::LineCircleBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void SDK::LineCircleBrush::SetColor(glm::vec3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void SDK::LineCircleBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void SDK::LineCircleBrush::Draw(GLfloat X, GLfloat Y, GLfloat Diameter, GLfloat Width, GLfloat OpacityValue) {
	Transform.Identity(ShapeMatrix);
	Opacity = 1.0f;

	Transform.Move(ShapeMatrix, X, Y);
	Opacity = OpacityValue;
	Radius = Diameter * 0.5;

	if(RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		WidthValue = Width / Camera.ZoomValue;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		WidthValue = Width;

	Render();
}

void SDK::LineCircleBrush::Render() {
	if (!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ShapeMatrix));

	gluDisk(SDK::SYSRES.GLU_LINE_CIRCLE, Radius - WidthValue * 0.5, Radius + WidthValue * 0.5, 80, 1);
}
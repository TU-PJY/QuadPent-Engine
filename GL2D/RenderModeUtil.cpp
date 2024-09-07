#include "RenderModeUtil.h"
#include "CameraUtil.h"

void RenderModeUtil::SetTextMode() {
	glUseProgram(TextShader);
	camera.SetCamera(false);
}

void RenderModeUtil::SetImageMode() {
	glUseProgram(ImageShader);
	camera.SetCamera(false);
}

void RenderModeUtil::SetStaticTextMode() {
	glUseProgram(TextShader);
	camera.SetCamera(true);
}

void RenderModeUtil::SetStaticImageMode() {
	glUseProgram(ImageShader);
	camera.SetCamera(true);
}
#include "RenderModeUtil.h"
#include "CameraUtil.h"

void RenderModeUtil::SetTextMode() {
	glUseProgram(TextShader);
	camera.SetCamera();
	camera.ProcessTransform(true);
}

void RenderModeUtil::SetImageMode() {
	glUseProgram(ImageShader);
	camera.SetCamera();
	camera.ProcessTransform(false);
}
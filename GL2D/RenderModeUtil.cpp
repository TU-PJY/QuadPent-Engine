#include "RenderModeUtil.h"
#include "CameraUtil.h"

void RenderModeUtil::SetTextMode() {
	glUseProgram(TextShader);
	cam.UseTextShader = true;
	cam.SetCamera();
}

void RenderModeUtil::SetImageMode() {
	glUseProgram(ImageShader);
	cam.SetCamera();
}
#pragma once
#include "GLHeader.h"
#include "Camera.h"

class RenderModeUtil {
public:
	void SetTextMode() {
		glUseProgram(TextShader);
		cam.UseTextShader = true;
		cam.SetCamera();
	}

	void SetImageMode() {
		glUseProgram(ImageShader);
		cam.SetCamera();
	}
};
extern RenderModeUtil renderMode;
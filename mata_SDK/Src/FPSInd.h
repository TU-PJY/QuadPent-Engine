#pragma once
#include "Object.h"

class FPS_Indicator : public Object {
private:
	RectBrush RECT{};
	TimerUtil FPS_IND_REFRESH_TIMER{};
	glm::vec2 RectPosition{};
	float CurrentDeltaTime = 1.0;

	TextUtil Text{};

public:
	FPS_Indicator() {
		Text.Init(SDK::FONT, FW_DONTCARE);
		Text.SetColor(1.0, 1.0, 1.0);
		Text.SetHeightAlign(HEIGHT_ALIGN_UNDER);

		RECT.SetRenderType(RENDER_TYPE_STATIC);
	}

	void UpdateFunc(float FrameTime) {
		FPS_IND_REFRESH_TIMER.Update(FrameTime);
		if (FPS_IND_REFRESH_TIMER.CheckMiliSec(0.5, 1, CHECK_AND_INTERPOLATE))
			CurrentDeltaTime = FrameTime;
	}

	void RenderIndicator() {
		UI.ClampPositionToCorner(EDGE_LEFT_UP, RectPosition.x, RectPosition.y, 0.25, 0.08, 0.0, 0.0);
		RECT.Draw(RectPosition.x, RectPosition.y, 0.25, 0.08, 0.0, 0.3);
		Text.Render(SDK::RECT.lx + 0.01, SDK::RECT.ry, 0.06, L"FPS: %d", (int)(round((1.0 / CurrentDeltaTime))));
	}
};
extern Object* Indicator;
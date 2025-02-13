#pragma once
#include "GameObject.h"

class FPS_Indicator : public GameObject {
private:
	RectBrush Rect{};
	TimerUtil FPS_IND_REFRESH_TIMER{};
	glm::vec2 RectPosition{};
	float CurrentDeltaTime = 1.0;

	TextUtil Text{};

public:
	FPS_Indicator() {
		Text.Init(SDK_FONT, FW_DONTCARE);
		Text.SetColor(1.0, 1.0, 1.0);
		Text.SetHeightAlign(HEIGHT_ALIGN_UNDER);

		Rect.SetRenderType(RENDER_TYPE_STATIC);
	}

	void UpdateFunc(float FrameTime) {
		FPS_IND_REFRESH_TIMER.Update(FrameTime);
		if (FPS_IND_REFRESH_TIMER.CheckMiliSec(0.5, 1, CHECK_AND_INTERPOLATE))
			CurrentDeltaTime = FrameTime;
	}

	void RenderIndicator() {
		UI.ClampPositionToCorner(EDGE_LEFT_UP, RectPosition.x, RectPosition.y, 0.25, 0.08, 0.0, 0.0);
		Rect.Draw(RectPosition.x, RectPosition.y, 0.25, 0.08, 0.0, 0.3);
		Text.Render(WindowRect.lx + 0.01, WindowRect.ry, 0.06, L"FPS: %d", (int)(round((1.0 / CurrentDeltaTime))));
	}
};
extern GameObject* Indicator;
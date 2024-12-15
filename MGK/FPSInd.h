#pragma once
#include "GameObject.h"

class FPSInd : public GameObject {
private:
	RectBrush Rect{};
	TextUtil Text{};
	TimerUtil FPS_IND_REFRESH_TIMER{};
	glm::vec2 RectPosition{};
	float CurrentDeltaTime = 1.0;

public:
	FPSInd() {
		Text.Init(L"Roboto", FW_NORMAL);
		Text.SetColor(1.0, 1.0, 1.0);
		Text.SetHeightAlign(HEIGHT_ALIGN_UNDER);
	}

	void UpdateFunc(float FT) {
		FPS_IND_REFRESH_TIMER.Update(FT);
		if (FPS_IND_REFRESH_TIMER.CheckMiliSec(0.5, 1, CHECK_AND_INTERPOLATE))
			CurrentDeltaTime = FT;
	}

	void RenderFunc() {
		UI::ClampPositionToCorner(EDGE_LEFT_UP, RectPosition.x, RectPosition.y, 0.25, 0.08, 0.0, 0.0);
		Rect.Draw(RectPosition.x, RectPosition.y, 0.25, 0.08, 0.0, 0.3);
		Text.Render(WindowRect.lx + 0.01, WindowRect.ry, 0.06, L"FPS: %d", (int)(round((1.0 / CurrentDeltaTime))));
	}
};
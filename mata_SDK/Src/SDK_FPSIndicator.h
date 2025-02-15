#pragma once
#include "SDK_Object.h"

class SDK_FPS_Indicator : public SDK::Object {
private:
	SDK::RectBrush Rect{};
	SDK::Timer FPS_IND_REFRESH_TIMER{};
	glm::vec2 RectPosition{};
	float CurrentDeltaTime = 1.0;

	SDK::Text IndicatorText{};

public:
	SDK_FPS_Indicator() {
		IndicatorText.Init(SDK::FONT, FW_DONTCARE);
		IndicatorText.SetColor(1.0, 1.0, 1.0);
		IndicatorText.SetHeightAlign(HEIGHT_ALIGN_UNDER);

		Rect.SetRenderType(RENDER_TYPE_STATIC);
	}

	void UpdateFunc(float FrameTime) {
		FPS_IND_REFRESH_TIMER.Update(FrameTime);
		if (FPS_IND_REFRESH_TIMER.CheckMiliSec(0.5, 1, CHECK_AND_INTERPOLATE))
			CurrentDeltaTime = FrameTime;
	}

	void RenderIndicator() {
		SDK::UITool.ClampPositionToCorner(EDGE_LEFT_UP, RectPosition.x, RectPosition.y, 0.25, 0.08, 0.0, 0.0);
		Rect.Draw(RectPosition.x, RectPosition.y, 0.25, 0.08, 0.0, 0.3);
		IndicatorText.Render(SDK::RECT.lx + 0.01, SDK::RECT.ry, 0.06, L"FPS: %d", (int)(round((1.0 / CurrentDeltaTime))));
	}
};
extern SDK::Object* Indicator;
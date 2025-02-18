#pragma once
#include "SDK_Object.h"

class SDK_FPS_Indicator : public MSDK::Object {
private:
	MSDK::Timer FPS_IND_REFRESH_TIMER{};
	float CurrentDeltaTime = 1.0;

	MSDK::Text IndicatorText{};

public:
	SDK_FPS_Indicator() {
		IndicatorText.Init(MSDK::FONT, FW_BOLD);
		IndicatorText.SetColor(0.0, 1.0, 0.0);
		IndicatorText.SetHeightAlign(HEIGHT_ALIGN_UNDER);
		IndicatorText.EnableShadow();
		IndicatorText.SetShadow(0.07, -0.07, 1.0);
	}

	void UpdateFunc(float FrameTime) {
		FPS_IND_REFRESH_TIMER.Update(FrameTime);
		if (FPS_IND_REFRESH_TIMER.CheckMiliSec(0.5, 1, CHECK_AND_INTERPOLATE))
			CurrentDeltaTime = FrameTime;
	}

	void RenderIndicator() {
		IndicatorText.Render(MSDK::WindowRect.LeftX + 0.01, MSDK::WindowRect.RightY, 0.1, L"FPS: %d", (int)(round((1.0 / CurrentDeltaTime))));
	}
};
extern MSDK::Object* Indicator;
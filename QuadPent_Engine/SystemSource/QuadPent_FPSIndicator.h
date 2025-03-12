#pragma once
#include "QuadPent_Object.h"

class SDK_FPS_Indicator : public QP::Object {
private:
	QP::Timer FPS_IND_REFRESH_TIMER{};
	float CurrentDeltaTime = 1.0;
	int MaxFPS{};

	QP::Text IndicatorText{};

public:
	SDK_FPS_Indicator() {
		IndicatorText.Init(QP::SYSRES.SYSTEM_FONT_BOLD);
		IndicatorText.SetColor(1.0, 1.0, 1.0);
		IndicatorText.SetHeightAlign(HEIGHT_ALIGN_UNDER);
		IndicatorText.EnableShadow();
		IndicatorText.SetShadow(0.07, -0.07, 1.0);

		DEVMODE DevMode;
		ZeroMemory(&DevMode, sizeof(DEVMODE));
		DevMode.dmSize = sizeof(DEVMODE);

		if (FRAME_LIMITS == QP::MAX_FRAMERATE) {
			if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode)) {
				std::cout << "Monitor Refresh Rate: " << DevMode.dmDisplayFrequency << " Hz" << std::endl;
				MaxFPS = static_cast<int>(DevMode.dmDisplayFrequency);
			}
		}
		else
			MaxFPS = FRAME_LIMITS;
	}

	void UpdateFunc(float FrameTime) {
		FPS_IND_REFRESH_TIMER.Update(FrameTime);
		if (FPS_IND_REFRESH_TIMER.CheckMiliSec(0.1, 1, CHECK_AND_INTERPOLATE))
			CurrentDeltaTime = FrameTime;
	}

	void RenderFunc() {
		int FPS = (int)(round((1.0 / CurrentDeltaTime)));
		float Result = (float)MaxFPS / (float)FPS;

		if (Result > 2.0)
			IndicatorText.SetMacroColor(1.0, 0.0, 0.0);
		else if (Result > 1.6)
			IndicatorText.SetMacroColor(1.0, 0.0, 1.0);
		else if (Result > 1.3)
			IndicatorText.SetMacroColor(1.0, 1.0, 0.0);
		else
			IndicatorText.SetMacroColor(0.0, 1.0, 0.0);

		QP::EXTool.ClampValue(FPS, 0, MaxFPS, CLAMP_FIXED);
		IndicatorText.Render(QP::WindowRect.LeftX + 0.01, QP::WindowRect.RightY + 0.02, 0.1, L"\\WFPS\\E \\P%d\\E", FPS);
	}
};
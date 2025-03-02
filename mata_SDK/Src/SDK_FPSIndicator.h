#pragma once
#include "SDK_Object.h"

class SDK_FPS_Indicator : public SDK::Object {
private:
	SDK::Timer FPS_IND_REFRESH_TIMER{};
	float CurrentDeltaTime = 1.0;
	int MaxFPS{};

	SDK::Text IndicatorText{};

public:
	SDK_FPS_Indicator() {
		IndicatorText.Init(SDK::SYSRES.SYSTEM_FONT_BOLD);
		IndicatorText.SetColor(1.0, 1.0, 1.0);
		IndicatorText.SetHeightAlign(HEIGHT_ALIGN_UNDER);
		IndicatorText.EnableShadow();
		IndicatorText.SetShadow(0.07, -0.07, 1.0);

		DEVMODE DevMode;
		ZeroMemory(&DevMode, sizeof(DEVMODE));
		DevMode.dmSize = sizeof(DEVMODE);

		if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode)) {
			std::cout << "Monitor Refresh Rate: " << DevMode.dmDisplayFrequency << " Hz" << std::endl;
			MaxFPS = static_cast<int>(DevMode.dmDisplayFrequency);
		}
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

		SDK::EXTool.ClampValue(FPS, 0, MaxFPS, CLAMP_FIXED);
		IndicatorText.Render(SDK::WindowRect.LeftX + 0.01, SDK::WindowRect.RightY, 0.1, L"\\WFPS\\E \\P%d\\E", FPS);
	}
};
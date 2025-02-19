#pragma once
#include "SDK_Object.h"

class SDK_IntroScreen : public SDK::Object {
private:
	SDK::Timer        IntroTimer;
	SDK::SoundChannel IntroChannel{};

	SDK::SinMove      IntroMove{}, OpacityMove{};

	float      LogoSize        = 1.0;
	float      LogoOpacity     = 0.0;
	float      LogoPosition    = -0.4;
	float      AnimationSpeed  = 6.0;

	int        SceneNumber{}; 

public:
	SDK_IntroScreen() {
		IntroMove.SetMovePoint(LogoPosition, 0.0);
		OpacityMove.SetMovePoint(0.0, 1.0);
	}

	void InputKey(SDK::KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN) {
			switch (Event.NormalKey) {
			case NK_ENTER:
				SDK::SoundTool.Stop(IntroChannel);
				SDK::Scene.SwitchMode(SDK::START_MODE);
				break;
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		IntroTimer.Update(FrameTime);
		switch (SceneNumber) {
		case 0:
			if (IntroTimer.CheckMiliSec(1.0, 1, CHECK_AND_INTERPOLATE)) {
				SDK::SoundTool.Play(SDK::SYSRES.INTRO_SOUND, IntroChannel);
				++SceneNumber;
			}
			break;
			

		case 1:
			if (IntroTimer.MiliSec() < 2.5) {
				IntroMove.Update(LogoPosition, AnimationSpeed, FrameTime);
				OpacityMove.Update(LogoOpacity, AnimationSpeed, FrameTime);
			}

			if (IntroTimer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
				LogoOpacity -= FrameTime * 2;
				SDK::EXTool.ClampValue(LogoOpacity, 0.0, CLAMP_LESS);
			}

			if (IntroTimer.CheckMiliSec(4.0, 1, CHECK_AND_INTERPOLATE)) {
				LogoPosition = -0.4;

				IntroMove.SetMovePoint(LogoPosition, 0.0);
				OpacityMove.SetMovePoint(0.0, 1.0);
				IntroMove.Reset();
				OpacityMove.Reset();

				++SceneNumber;
			}
			break;


		case 2:
			if (IntroTimer.MiliSec() < 2.5) {
				IntroMove.Update(LogoPosition, AnimationSpeed, FrameTime);
				OpacityMove.Update(LogoOpacity, AnimationSpeed, FrameTime);
			}

			if (IntroTimer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
				LogoOpacity -= FrameTime * 2;
				SDK::EXTool.ClampValue(LogoOpacity, 0.0, CLAMP_LESS);
			}

			if (IntroTimer.CheckMiliSec(4.0, 1, CHECK_AND_RESUME))
				SDK::Scene.SwitchMode(SDK::START_MODE);
			
			break;
		}
	}

	void RenderFunc() {
		Begin(RENDER_TYPE_STATIC);
		SDK::Transform.Move(SDK::MoveMatrix, 0.0, LogoPosition);
		SDK::ImageTool.SetColor(1.0, 1.0, 1.0);

		switch (SceneNumber) {
		case 1:
			SDK::ImageTool.RenderImage(SDK::SYSRES.SDK_LOGO, LogoOpacity);
			break;

		case 2: case 3:
			SDK::ImageTool.RenderImage(SDK::SYSRES.FMOD_LOGO, LogoOpacity);
			break;
		}
	}
};
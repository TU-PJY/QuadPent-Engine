#pragma once
#include "SDK_Object.h"

class SDK_IntroScreen : public MSDK::Object {
private:
	MSDK::Timer    timer;
	MSDK::SoundChannel IntroChannel{};

	float      LogoSize        = 1.0;
	float      LogoOpacity     = 0.0;
	float      LogoPosition    = -0.3;
	float      AnimationSpeed  = 4.0;

	int          SceneNumber{}; 

public:
	void InputKey(MSDK::KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN) {
			switch (Event.NormalKey) {
			case NK_ENTER:
				MSDK::SoundTool.Stop(IntroChannel);
				MSDK::Scene.SwitchMode(MSDK::START_MODE);
				break;
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		timer.Update(FrameTime);
		switch (SceneNumber) {
		case 0:
			if (timer.CheckMiliSec(1.0, 1, CHECK_AND_INTERPOLATE)) {
				MSDK::SoundTool.Play(MSDK::SYSRES.INTRO_SOUND, IntroChannel);
				++SceneNumber;
			}
			break;
			

		case 1:
			if (timer.MiliSec() < 2.5) {
				MSDK::Math.Lerp(LogoPosition, 0.0, AnimationSpeed, FrameTime);
				MSDK::Math.Lerp(LogoOpacity, 1.0, AnimationSpeed, FrameTime);
			}

			if (timer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
				LogoOpacity -= FrameTime * 2;
				MSDK::EXTool.ClampValue(LogoOpacity, 0.0, CLAMP_LESS);
			}

			if (timer.CheckMiliSec(4.0, 1, CHECK_AND_INTERPOLATE)) {
				LogoPosition = -0.3;
				++SceneNumber;
			}
			break;


		case 2:
			if (timer.MiliSec() < 2.5) {
				MSDK::Math.Lerp(LogoPosition, 0.0, AnimationSpeed, FrameTime);
				MSDK::Math.Lerp(LogoOpacity, 1.0, AnimationSpeed, FrameTime);
			}

			if (timer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
				LogoOpacity -= FrameTime * 2;
				MSDK::EXTool.ClampValue(LogoOpacity, 0.0, CLAMP_LESS);
			}

			if (timer.CheckMiliSec(4.0, 1, CHECK_AND_RESUME))
				MSDK::Scene.SwitchMode(MSDK::START_MODE);
			
			break;
		}
	}

	void RenderFunc() {
		Begin(RENDER_TYPE_STATIC);
		MSDK::Transform.Move(MSDK::MoveMatrix, 0.0, LogoPosition);

		switch (SceneNumber) {
		case 1:
			MSDK::ImageTool.RenderImage(MSDK::SYSRES.SDK_LOGO, LogoOpacity);
			break;

		case 2: case 3:
			MSDK::ImageTool.RenderImage(MSDK::SYSRES.FMOD_LOGO, LogoOpacity);
			break;
		}
	}
};
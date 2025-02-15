#pragma once
#include "Object.h"

class IntroScreen : public SDK::Object {
private:
	SDK::Timer    timer;
	SoundChannel IntroChannel{};

	GLfloat      LogoSize        = 1.0;
	GLfloat      LogoOpacity     = 0.0;
	GLfloat      LogoPosition    = -0.3;
	GLfloat      AnimationSpeed  = 4.0;

	int          SceneNumber{}; 

public:
	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN) {
			switch (Event.NormalKey) {
			case NK_ENTER:
				SDK::SoundTool.Stop(IntroChannel);
				SDK::Scene.SwitchMode(SDK::START_MODE);
				break;

#ifdef ENABLE_DEV_EXIT
			case NK_ESCAPE:
				System.Exit();
				break;
#endif
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		timer.Update(FrameTime);
		switch (SceneNumber) {
		case 0:
			if (timer.CheckMiliSec(1.0, 1, CHECK_AND_INTERPOLATE)) {
				SDK::SoundTool.Play(SDK::SYSRES.INTRO_SOUND, IntroChannel);
				++SceneNumber;
			}
			break;
			

		case 1:
			if (timer.MiliSec() < 2.5) {
				SDK::Math.Lerp(LogoPosition, 0.0, AnimationSpeed, FrameTime);
				SDK::Math.Lerp(LogoOpacity, 1.0, AnimationSpeed, FrameTime);
			}

			if (timer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
				LogoOpacity -= FrameTime * 2;
				SDK::EXTool.ClampValue(LogoOpacity, 0.0, CLAMP_LESS);
			}

			if (timer.CheckMiliSec(4.0, 1, CHECK_AND_INTERPOLATE)) {
				LogoPosition = -0.3;
				++SceneNumber;
			}
			break;


		case 2:
			if (timer.MiliSec() < 2.5) {
				SDK::Math.Lerp(LogoPosition, 0.0, AnimationSpeed, FrameTime);
				SDK::Math.Lerp(LogoOpacity, 1.0, AnimationSpeed, FrameTime);
			}

			if (timer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
				LogoOpacity -= FrameTime * 2;
				SDK::EXTool.ClampValue(LogoOpacity, 0.0, CLAMP_LESS);
			}

			if (timer.CheckMiliSec(4.0, 1, CHECK_AND_RESUME))
				SDK::Scene.SwitchMode(SDK::START_MODE);
			
			break;
		}
	}

	void RenderFunc() {
		Begin(RENDER_TYPE_STATIC);
		SDK::Transform.Move(MoveMatrix, 0.0, LogoPosition);

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
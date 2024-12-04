#pragma once
#include "Scene.h"
#include "EngineHeader.h"

class IntroScreen : public GameObject {
private:
	TimerUtil    timer;
	SoundChannel IntroChannel{};

	GLfloat      LogoSize        = 1.0;
	GLfloat      LogoTransparent = 0.0;
	GLfloat      LogoPosition    = -0.3;
	GLfloat      AnimationSpeed  = 5.0;

	int          Scene{}; 

public:
	void InputKey(int State, unsigned char NormalKey, int SpecialKey) {
		if (State == NORMAL_KEY_DOWN) {
			switch (NormalKey) {
			case NK_ENTER:
				StopSound(IntroChannel);
				scene.SwitchMode(START_MODE);
				break;

			case NK_ESCAPE:
				Framework::Exit();
				break;
			}
		}
	}

	void UpdateFunc(float FT) {
		timer.Update(FT);
		switch (Scene) {
		case 0:
			if (timer.CheckMiliSec(1.0, 1, CHECK_AND_INTERPOLATE)) {
				soundUtil.PlaySound(IntroSound, IntroChannel);
				++Scene;
			}
			break;
			

		case 1:
			if (timer.MiliSec() < 2.5) {
				Math::UpdateLerp(LogoPosition, 0.0, AnimationSpeed, FT);
				Math::UpdateLerp(LogoTransparent, 1.0, AnimationSpeed, FT);
			}

			if (timer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
				LogoTransparent -= FT * 2;
				EX::ClampValue(LogoTransparent, 0.0, CLAMP_LESS);
			}

			if (timer.CheckMiliSec(4.0, 1, CHECK_AND_INTERPOLATE)) {
				LogoPosition = -0.3;
				++Scene;
			}
			break;


		case 2:
			if (timer.MiliSec() < 2.5) {
				Math::UpdateLerp(LogoPosition, 0.0, AnimationSpeed, FT);
				Math::UpdateLerp(LogoTransparent, 1.0, AnimationSpeed, FT);
			}

			if (timer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
				LogoTransparent -= FT * 2;
				EX::ClampValue(LogoTransparent, 0.0, CLAMP_LESS);
			}

			if (timer.CheckMiliSec(4.0, 1, CHECK_AND_RESUME))
				scene.SwitchMode(START_MODE);
			
			break;
		}
	}

	void RenderFunc() {
		switch (Scene) {
		case 1:
			RenderImage(RENDER_TYPE_STATIC, MGK_LOGO, 0.0, LogoPosition, 1.0, 1.0, 0.0, LogoTransparent);
			break;

		case 2: case 3:
			RenderImage(RENDER_TYPE_STATIC, FMOD_LOGO, 0.0, LogoPosition, 1.0, 1.0, 0.0, LogoTransparent);
			break;
		}
	}
};
#pragma once
#include "Scene.h"
#include "EngineHeader.h"
#include <cmath>

class IntroScreen : public GameObject {
private:
	TimerUtil    timer;

	SoundChannel IntroChannel{};

	GLfloat      LogoSize        = 1.0;
	GLfloat      LogoTransparent = 1.0;
	GLfloat      LogoPosition    = -0.5;
	GLfloat      RectPosition    = -0.5;

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
			RectPosition -= FT * 1.5;
			LogoPosition += FT * 1.5;
			EX::ClampValue(RectPosition, -1.0, CLAMP_LESS);
			EX::ClampValue(LogoPosition, 0.0, CLAMP_GREATER);

			if (timer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
				LogoTransparent -= FT * 2;
				EX::ClampValue(LogoTransparent, 0.0, CLAMP_LESS);
			}

			if (timer.CheckMiliSec(4.0, 1, CHECK_AND_INTERPOLATE)) {
				RectPosition = -0.5;
				LogoPosition = -0.5;
				LogoTransparent = 1.0;
				++Scene;
			}
			break;


		case 2:
			RectPosition -= FT * 1.5;
			LogoPosition += FT * 1.5;
			EX::ClampValue(RectPosition, -1.0, CLAMP_LESS);
			EX::ClampValue(LogoPosition, 0.0, CLAMP_GREATER);

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
		AlphaClipping::First();
		RenderImage(RENDER_TYPE_STATIC, COLOR_TEXTURE, RectPosition, 0.0, 1.0, 1.0);

		AlphaClipping::Second();

		switch (Scene) {
		case 1:
			RenderImage(RENDER_TYPE_STATIC, MGK_LOGO, LogoPosition, 0.0, 1.0, 1.0, 0.0, LogoTransparent);
			break;

		case 2: case 3:
			RenderImage(RENDER_TYPE_STATIC, FMOD_LOGO, LogoPosition, 0.0, 1.0, 1.0, 0.0, LogoTransparent);
			break;
		}

		AlphaClipping::End();
	}
};
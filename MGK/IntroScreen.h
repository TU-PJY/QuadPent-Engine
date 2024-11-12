#pragma once
#include "Scene.h"
#include "EngineHeader.h"
#include <cmath>

class IntroScreen : public GameObject {
private:
	TimerUtil timer;

	SoundChannel IntroChannel{};

	GLfloat LogoSize = 1.0;
	GLfloat LogoTransparent = 1.0;
	GLfloat LogoPosition = -0.5;
	GLfloat RectPosition = -0.5;

	int Scene{};

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
			if (timer.Sec() >= 1) {
				soundUtil.PlaySound(IntroSound, IntroChannel);
				timer.Reset();
				++Scene;
			}
			break;
			

		case 1:
			RectPosition -= FT * 1.5;
			LogoPosition += FT * 1.5;
			EX::ClampValue(RectPosition, -1.0, CLAMP_LESS);
			EX::ClampValue(LogoPosition, 0.0, CLAMP_GREATER);

			if (timer.MiliSec() >= 2.5) {
				LogoTransparent -= FT * 2;
				EX::ClampValue(LogoTransparent, 0.0, CLAMP_LESS);
			}

			if (timer.MiliSec() >= 4) {
				RectPosition = -0.5;
				LogoPosition = -0.5;
				LogoTransparent = 1.0;
				timer.Reset();
				++Scene;
			}
			break;


		case 2:
			RectPosition -= FT * 1.5;
			LogoPosition += FT * 1.5;
			EX::ClampValue(RectPosition, -1.0, CLAMP_LESS);
			EX::ClampValue(LogoPosition, 0.0, CLAMP_GREATER);

			if (timer.MiliSec() >= 2.5) {
				LogoTransparent -= FT * 2;
				EX::ClampValue(LogoTransparent, 0.0, CLAMP_LESS);
			}

			if (timer.Sec() >= 4)
				scene.SwitchMode(START_MODE);
			
			break;
		}
	}

	void RenderFunc() {
		AlphaClipping::First();
		InitRenderState(RENDER_TYPE_STATIC);
		Transform::Move(TranslateMatrix, RectPosition, 0.0);
		Render(COLOR_TEXTURE);

		AlphaClipping::Second();
		InitRenderState(RENDER_TYPE_STATIC);
		Transform::Move(TranslateMatrix, LogoPosition, 0.0);

		switch (Scene) {
		case 1:
			Render(MGK_LOGO, LogoTransparent);
			break;

		case 2: case 3:
			Render(FMOD_LOGO, LogoTransparent);
			break;
		}

		AlphaClipping::End();
	}
};
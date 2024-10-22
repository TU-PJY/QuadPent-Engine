#pragma once
#include "Framework.h"
#include "EngineHeader.h"
#include <cmath>

class IntroScreen : public GameObject {
private:
	TimerUtil timer;

	SoundChannel IntroChannel{};

	GLfloat LogoSize = 2.0;
	GLfloat LogoTransparent = 1.0;
	GLfloat Rotation{};
	GLfloat RotateValue = 40.0;

	PopBounce PBA;
	SinLoop LSA;

	int Scene{};

public:
	IntroScreen() {
		SetColor(1.0, 1.0, 1.0);
	}

	void InputKey(int Type, int State, unsigned char NormalKey, int SpecialKey) {
		if (Type == KEY_TYPE_NORMAL && State == KEY_DOWN) {
			switch (NormalKey) {
			case 13:
				StopSound(IntroChannel);
				framework.SwitchMode(StartMode);
				break;

			case 27:
				framework.Exit();
				break;
			}
		}
	}

	void Update(float FT) {
		timer.Update(FT);

		if (timer.Sec() >= 1 && Scene == 0) {
			soundUtil.PlaySound(IntroSound, IntroChannel);
			++Scene;
		}

		if (Scene == 1) {
			LogoSize = PBA.Update(1.0, 2.0, 8.0, 8.0, 5.0, FT);
			Rotation = LSA.Update(RotateValue, 10, FT);
			RotateValue = Math::Lerp(RotateValue, 0.0, 4, FT);
		}

		if (timer.Sec() >= 3 && Scene == 1) {
			LogoTransparent -= FT * 2;
			if (LogoTransparent <= 0) {
				LogoTransparent = 0;
				LogoSize = 2.0;
				++Scene;
			}
		}

		if (Scene == 2) {
			if (timer.Sec() >= 4) {
				LogoTransparent = Math::Lerp(LogoTransparent, 1.0, 10, FT);
				LogoSize = Math::Lerp(LogoSize, 1.0, 10, FT);
				if (timer.Sec() >= 6)
					++Scene;
			}
		}

		if (Scene == 3) {
			LogoTransparent -= FT * 2;
			if (LogoTransparent <= 0)
				LogoTransparent = 0;
		}

		if (timer.Sec() >= 7 && Scene == 3)
			framework.SwitchMode(StartMode);
	}

	void Render() {
		InitMatrix(RENDER_TYPE_STATIC);

		if (Scene == 1) {
			Transform::Rotate(RotateMatrix, Rotation);
			Transform::Scale(ScaleMatrix, LogoSize, LogoSize);
			RenderImage(ImageEngineLogo, LogoTransparent);
		}

		else if (Scene == 2 || Scene == 3) {
			Transform::Scale(ScaleMatrix, LogoSize, LogoSize);
			RenderImage(ImageFMODLogo, LogoTransparent);
		}
	}
};


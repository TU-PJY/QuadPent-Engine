#pragma once
#include "Framework.h"
#include <cmath>

class IntroScreen : public GameObject {
private:
	TimerUtil timer;

	Channel IntroChannel{};

	GLfloat LogoSize = 2.0;
	GLfloat LogoTransparent = 1.0;
	GLfloat Rotation{};
	GLfloat RotateValue = 40.0;

	PopBounceAnimation PBA;
	LoopSinAnimation LSA;

	int Scene{};

public:
	IntroScreen() {
		SetColor(1.0, 1.0, 1.0);
	}

	void InputKey(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey) {
		if (Type == KeyType::Normal && State == KeyState::Down) {
			if (NormalKey == 13) {
				StopSound(IntroChannel);
				SetBackColor(R, G, B);
				framework.SwitchMode(StartMode);
			}

			else if(NormalKey == 27)
				framework.Exit();
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
			Rotation = LSA.Update(RotateValue, FT * 10);
			RotateValue = Math::Lerp(RotateValue, 0.0, FT * 4);
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
				LogoTransparent = Math::Lerp(LogoTransparent, 1.0, FT * 10);
				LogoSize = Math::Lerp(LogoSize, 1.0, FT * 10);
				if (timer.Sec() >= 6)
					++Scene;
			}
		}

		if (Scene == 3) {
			LogoTransparent -= FT * 2;
			if (LogoTransparent <= 0)
				LogoTransparent = 0;
		}

		if (timer.Sec() >= 7 && Scene == 3) {
			SetBackColor(R, G, B);
			framework.SwitchMode(StartMode);
		}
	}

	void Render() {
		InitMatrix(RenderType::Static);

		if (Scene == 1) {
			Transform::Rotate(RotateMatrix, Rotation);
			Transform::Scale(ScaleMatrix, LogoSize, LogoSize);
			RenderImage(ImageEngineLogo, LogoTransparent);
		}

		else if (Scene == 2 || Scene == 3) {
			Transform::Scale(ScaleMatrix, LogoSize, LogoSize);
			Transform::MatchAspect(ScaleMatrix, 2000, 800);
			RenderImage(ImageFMODLogo, LogoTransparent);
		}
	}
};


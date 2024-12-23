#pragma once
#include "Scene.h"
#include "EngineHeader.h"

class IntroScreen : public GameObject {
private:
	TimerUtil    timer;
#ifdef USE_SOUND_SYSTEM
	SoundChannel IntroChannel{};
#endif

	GLfloat      LogoSize        = 1.0;
	GLfloat      LogoTransparent = 0.0;
	GLfloat      LogoPosition    = -0.3;
	GLfloat      AnimationSpeed  = 0.5;
	GLfloat      BlurValue = 5.0;

	SinLerp      Slerp{};
	SinLerp      BlurLerp{};

	int          Scene{}; 

public:
	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN) {
			switch (Event.NormalKey) {
			case NK_ENTER:
#ifdef USE_SOUND_SYSTEM
				StopSound(IntroChannel);
#endif
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
#ifdef USE_SOUND_SYSTEM
				soundUtil.PlaySound(SysRes.INTRO_SOUND, IntroChannel);
#endif
				++Scene;
			}
			break;
			

		case 1:
			if (timer.MiliSec() < 2.5) {
				LogoPosition = Slerp.Update(LogoPosition, 0.0, AnimationSpeed, FT);
				LogoTransparent = Slerp.Update(LogoTransparent, 1.0, AnimationSpeed, FT);
				BlurValue = BlurLerp.Update(BlurValue, 0.0, AnimationSpeed, FT);
			}

			if (timer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
				LogoTransparent -= FT * 2;
				EX::ClampValue(LogoTransparent, 0.0, CLAMP_LESS);
			}

			if (timer.CheckMiliSec(4.0, 1, CHECK_AND_INTERPOLATE)) {
				LogoPosition = -0.3;
				BlurValue = 5.0;
				Slerp.Reset();
				BlurLerp.Reset();
				++Scene;
			}
			break;


		case 2:
			if (timer.MiliSec() < 2.5) {
				LogoPosition = Slerp.Update(LogoPosition, 0.0, AnimationSpeed, FT);
				LogoTransparent = Slerp.Update(LogoTransparent, 1.0, AnimationSpeed, FT);
				BlurValue = BlurLerp.Update(BlurValue, 0.0, AnimationSpeed, FT);
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
		ResetUnitTransform();
		SetUnitBlur(BlurValue);

		switch (Scene) {
		case 1:
			DrawImage(RENDER_TYPE_STATIC, SysRes.MGK_LOGO, 0.0, LogoPosition, 1.0, 1.0, 0.0, LogoTransparent, FLIP_TYPE_NONE, true);
			break;

		case 2: case 3:
			DrawImage(RENDER_TYPE_STATIC, SysRes.FMOD_LOGO, 0.0, LogoPosition, 1.0, 1.0, 0.0, LogoTransparent, FLIP_TYPE_NONE, true);
			break;
		}
	}
};
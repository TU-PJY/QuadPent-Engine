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
	GLfloat      LogoOpacity = 0.0;
	GLfloat      LogoPosition    = -0.3;
	GLfloat      AnimationSpeed  = 0.5;

	SinLerp      Slerp{};

	int          Scene{}; 

public:
	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN) {
			switch (Event.NormalKey) {
			case NK_ENTER:
#ifdef USE_SOUND_SYSTEM
				soundUtil.StopSound(IntroChannel);
#endif
				scene.SwitchMode(START_MODE);
				break;

#ifdef ENABLE_DEV_EXIT
			case NK_ESCAPE:
				Framework::Exit();
				break;
#endif
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		timer.Update(FrameTime);
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
				LogoPosition = Slerp.Update(LogoPosition, 0.0, AnimationSpeed, FrameTime);
				LogoOpacity = Slerp.Update(LogoOpacity, 1.0, AnimationSpeed, FrameTime);
			}

			if (timer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
				LogoOpacity -= FrameTime * 2;
				EX.ClampValue(LogoOpacity, 0.0, CLAMP_LESS);
			}

			if (timer.CheckMiliSec(4.0, 1, CHECK_AND_INTERPOLATE)) {
				LogoPosition = -0.3;
				Slerp.Reset();
				++Scene;
			}
			break;


		case 2:
			if (timer.MiliSec() < 2.5) {
				LogoPosition = Slerp.Update(LogoPosition, 0.0, AnimationSpeed, FrameTime);
				LogoOpacity = Slerp.Update(LogoOpacity, 1.0, AnimationSpeed, FrameTime);
			}

			if (timer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
				LogoOpacity -= FrameTime * 2;
				EX.ClampValue(LogoOpacity, 0.0, CLAMP_LESS);
			}

			if (timer.CheckMiliSec(4.0, 1, CHECK_AND_RESUME))
				scene.SwitchMode(START_MODE);
			
			break;
		}
	}

	void RenderFunc() {
		BeginRender(RENDER_TYPE_STATIC);
		transform.Move(TranslateMatrix, 0.0, LogoPosition);

		switch (Scene) {
		case 1:
			RenderSprite(SysRes.MGK_LOGO, LogoOpacity);
			break;

		case 2: case 3:
			RenderSprite(SysRes.FMOD_LOGO, LogoOpacity);
			break;
		}
	}
};
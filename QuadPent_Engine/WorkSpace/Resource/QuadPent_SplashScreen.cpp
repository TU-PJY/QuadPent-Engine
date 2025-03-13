#include <QuadPent_StartUp.h>
// customize your splash screen

float SPLASH_TIME_DURATION_SEC = 3;

float Opacity = 0.0;

void QuadPent_StartUp::UpdateSplashScreen(float FrameTime) {
	SplashTimer.Update(FrameTime);
	if (SplashTimer.CheckSec(SPLASH_TIME_DURATION_SEC, CHECK_AND_STOP)) {
		SetWindowDefault();

		if (START_WITH_FULLSCREEN)
			QP::System.SwitchScreenState();

		SplashState = false;
	}
	
	Opacity += FrameTime;
	QP::EXTool.ClampValue(Opacity, 1.0, CLAMP_GREATER);
}

void QuadPent_StartUp::RenderSplashScreen() {
	if(SplashTimer.MiliSec() < (float)SPLASH_TIME_DURATION_SEC - 0.1) {
		QP::Begin(RENDER_TYPE_STATIC);
		QP::Transform.Scale(2.5, 2.5);
		QP::ImageTool.SetLocalColor(1.0, 1.0, 1.0);
		QP::ImageTool.RenderImage(QP::SYSRES.QUADPENT_LOGO, Opacity);
	}
}
#include <QuadPent_StartUp.h>
// customize your splash screen

float SPLASH_TIME_DURATION_SEC = 3;

void QuadPent_StartUp::UpdateSplashScreen(float FrameTime) {
	SplashTimer.Update(FrameTime);
	if (SplashTimer.CheckSec(SPLASH_TIME_DURATION_SEC, CHECK_AND_STOP)) {
		
		///////////////////////////////////
		// Do not erase these
		SplashState = false;
		SwitchingState = true;
	}
}

void QuadPent_StartUp::RenderSplashScreen() {
	QP::Begin(RENDER_TYPE_STATIC);
	QP::Transform.Scale(2.5, 2.5);
	QP::ImageTool.SetLocalColor(1.0, 1.0, 1.0);
	QP::ImageTool.RenderImage(QP::SYSRES.QUADPENT_LOGO);
}

void QuadPent_StartUp::SwitchToStartUp() {
	// Do not erase this
	SetWindowDefault();
	///////////////////////////////////
	if (START_WITH_FULLSCREEN)
		QP::System.SwitchScreenState();




	///////////////////////////////////
	// Do not erase this
	SwitchingState = false;
}
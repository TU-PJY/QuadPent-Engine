#pragma once
#include "QuadPent_Scene.h"
#include "QuadPent_FPSIndicator.h"

class QuadPent_StartUp : public QP::Object {
private:
	bool    StartUpPrepared{};
	bool    SplashState{};
	bool    SwitchingState{};

	bool		      ComponentLoadThreadCreated{};
	QP::ThreadHandle  SystemResourceLoadHandle{};
	QP::ThreadHandle  ImageResourceLoadHandle{};
	QP::ThreadHandle  SoundResourceLoadHandle{};
	QP::ThreadHandle  DataResourceLoadHandle{};
	QP::ThreadHandle  FontResourceLoadHandle{};

	bool    SystemResourceLoadEnd{};
	bool    ImageResourceLoadEnd{};
	bool    SoundResourceLoadEnd{};
	bool    DataResourceLoadEnd{};
	bool    FontResourceLoadEnd{};
	bool    InitializationEnd{};

	QP::Timer         SplashTimer{};
	QP::Timer         StartUpTimer{};
	QP::Timer         StartTimer{};
	QP::RectBrush     ScreenRect{};
	QP::LineBrush     ProgressLine{};

	QP::SoundChannel  SndChannel{};
	bool              LogoSoundPlayed{};

	float             LogoRotation{ -180.0 };
	float             LogoSize{ 2.5 };
	float             LogoOpacity{};

	std::atomic<int>  LoadingProgress{};
	float             LoadingProgressLength{};
	float             ProgressBarOpacity{};

	float			  ScreenOpacity{ 1.0 };

	bool			  RenderLogo{};
	bool			  ExitState{};

public:
	QuadPent_StartUp() {
		ScreenRect.SetColor(0.0, 0.0, 0.0);
		ScreenRect.SetRenderType(RENDER_TYPE_STATIC);
		ProgressLine.SetColor(1.0, 1.0, 1.0);
		ProgressLine.SetRenderType(RENDER_TYPE_STATIC);

		if (ENABLE_START_UP_SCREEN && ENABLE_SPLASH_SCREEN)
			SplashState = true;
		else
			SplashState = false;

		if (ENABLE_DEV_MODE)
			SplashState = false;
	}

	void UpdateSplashScreen(float FrameTime);
	void RenderSplashScreen();
	void SwitchToStartUp();

	void UpdateFunc(float FrameTime) {
		if(!StartUpPrepared) 
			PrepareStartUp();

		else {
			if (ENABLE_START_UP_SCREEN && ENABLE_SPLASH_SCREEN && !ENABLE_DEV_MODE) {
				if (SplashState)
					UpdateSplashScreen(FrameTime);
				else {
					if (SwitchingState)
						SwitchToStartUp();
					UpdateStartUp(FrameTime);
				}
			}

			else if (ENABLE_START_UP_SCREEN && !ENABLE_SPLASH_SCREEN && !ENABLE_DEV_MODE)
				UpdateStartUp(FrameTime);
			
			else if (ENABLE_DEV_MODE || !ENABLE_START_UP_SCREEN) 
				SkipStartUp();
		}
	}

	void PrepareStartUp() {
		QP::Camera.Init();
		QP::ImageTool.Init();
		QP::SoundTool.Init();

		QP::FontLoader.Load(QP::SYSRES.SYSTEM_FONT_DIRECTORY);
		QP::SYSRES.SYSTEM_FONT_REGULAR.Init(L"Space Grotesk");
		QP::SYSRES.SYSTEM_FONT_BOLD.Init(L"Space Grotesk", FW_BOLD);

		QP::ImageTool.LoadImage(QP::SYSRES.MATA_LOGO, QP::SYSRES.MATA_LOGO_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
		QP::ImageTool.LoadImage(QP::SYSRES.QUADPENT_LOGO, QP::SYSRES.QUADPENT_LOGO_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
		QP::SoundTool.Load(QP::SYSRES.INTRO_SOUND, QP::SYSRES.QUADPENT_LOGO_SOUND_DIRECTORY);

		ASSET::ResourcePreLoader();
		ASSET::StartUpBehavior();
		std::cout << "Resources pre-loaded." << std::endl;

		StartUpPrepared = true;
	}

	void UpdateStartUp(float FrameTime) {
		StartUpTimer.Update(FrameTime);
		if (StartUpTimer.CheckSec(1, CHECK_AND_RESUME)) {
			RenderLogo = true;
			QP::Math.Lerp(LogoSize, 0.6, 3.0, FrameTime);
			QP::Math.Lerp(LogoRotation, 0.0, 3.0, FrameTime);
			QP::Math.Lerp(LogoOpacity, 1.0, 3.0, FrameTime);
			QP::SoundTool.PlayOnce(QP::SYSRES.INTRO_SOUND, SndChannel, LogoSoundPlayed);
		}

		if (StartUpTimer.CheckSec(6, CHECK_AND_STOP)) {
			if (!ComponentLoadThreadCreated) {
				QP::ThreadTool.Create(SystemResourceLoadHandle, SystemResourceLoader);
				ComponentLoadThreadCreated = true;
			}

			if (!ExitState)
				QP::Math.Lerp(ProgressBarOpacity, 1.0, 5.0, FrameTime);

			QP::Math.Lerp(LoadingProgressLength, (float)LoadingProgress, 8.0, FrameTime);

			if (LoadResources()) {
				if (!InitializationEnd) {
					QP::ImageTool.Map();
					std::cout << "All of Image resources mapped." << std::endl;

					QP::ModeMapper();
					std::cout << "All of Mode pointers mapped." << std::endl;

					InitializationEnd = true;
				}

				else {
					if (LoadingProgress == 5 && StartTimer.UpdateAndCheckSec(1, CHECK_AND_STOP, FrameTime)) {
						if (!ExitState) {
							if (ENABLE_DEV_MODE && ENABLE_FPS_INDICATOR)
								QP::Scene.AddSystemObject(new QuadPent_FPS_Indicator);
							QP::Scene.SwitchMode(QP::START_MODE);
							ExitState = true;
						}
						else {
							ProgressBarOpacity -= FrameTime;
							if (QP::EXTool.CheckClampValue(ProgressBarOpacity, 0.0, CLAMP_LESS)) {
								ScreenOpacity -= FrameTime * 0.7;
								if (QP::EXTool.CheckClampValue(ScreenOpacity, 0.0, CLAMP_LESS))
									QP::Scene.DeleteObject(this);
							}
						}
					}
				}
			}
		}
	}

	void SkipStartUp() {
		if (!ComponentLoadThreadCreated) {
			QP::ThreadTool.Create(SystemResourceLoadHandle, SystemResourceLoader);
			ComponentLoadThreadCreated = true;
		}

		if (LoadResources()) {
			if (!InitializationEnd) {
				QP::ImageTool.Map();
				std::cout << "All of Image resources mapped." << std::endl;

				QP::ModeMapper();
				std::cout << "All of Mode pointers mapped." << std::endl;

				if (ENABLE_DEV_MODE && ENABLE_FPS_INDICATOR)
					QP::Scene.AddSystemObject(new QuadPent_FPS_Indicator);
				QP::Scene.SwitchMode(QP::START_MODE);
				QP::Scene.DeleteObject(this);
				InitializationEnd = true;
			}
		}
	}

	bool LoadResources() {
		if (!SystemResourceLoadEnd && !QP::ThreadTool.CheckRunning(SystemResourceLoadHandle)) {
			QP::ThreadTool.Close(SystemResourceLoadHandle);
			QP::ThreadTool.Create(ImageResourceLoadHandle, ASSET::ImageResourceLoader);
			QP::ThreadTool.Create(SoundResourceLoadHandle, ASSET::SoundResourceLoader);
			QP::ThreadTool.Create(DataResourceLoadHandle, ASSET::FileResourceLoader);
			QP::ThreadTool.Create(FontResourceLoadHandle, ASSET::FontResourceLoader);
			std::cout << "System resource load completed." << std::endl;

			LoadingProgress.fetch_add(1);
			SystemResourceLoadEnd = true;
		}

		if (SystemResourceLoadEnd) {
			if (!ImageResourceLoadEnd && !QP::ThreadTool.CheckRunning(ImageResourceLoadHandle)) {
				QP::ThreadTool.Close(ImageResourceLoadHandle);
				std::cout << "Image resource load completed." << std::endl;

				LoadingProgress.fetch_add(1);
				ImageResourceLoadEnd = true;
			}

			if (!SoundResourceLoadEnd && !QP::ThreadTool.CheckRunning(SoundResourceLoadHandle)) {
				QP::ThreadTool.Close(SoundResourceLoadHandle);
				std::cout << "Sound resource load completed." << std::endl;

				LoadingProgress.fetch_add(1);
				SoundResourceLoadEnd = true;
			}

			if (!DataResourceLoadEnd && !QP::ThreadTool.CheckRunning(DataResourceLoadHandle)) {
				QP::ThreadTool.Close(DataResourceLoadHandle);
				std::cout << "Data resource load completed." << std::endl;

				LoadingProgress.fetch_add(1);
				DataResourceLoadEnd = true;
			}

			if (!FontResourceLoadEnd && !QP::ThreadTool.CheckRunning(FontResourceLoadHandle)) {
				QP::ThreadTool.Close(FontResourceLoadHandle);
				std::cout << "Font resource load completed." << std::endl;

				ASSET::FontResourceInitializer();
				std::cout << "Font resources created." << std::endl;

				LoadingProgress.fetch_add(1);
				FontResourceLoadEnd = true;
			}

			if (SystemResourceLoadEnd && ImageResourceLoadEnd && SoundResourceLoadEnd && DataResourceLoadEnd && FontResourceLoadEnd)
				return true;
		}

		return false;
	}

	void SetWindowDefaultStyle() {
		if (QP::SYSTEM_TITLE_ICON) {
			SendMessage(QP::System_HWND, WM_SETICON, ICON_SMALL, (LPARAM)QP::SYSTEM_TITLE_ICON);
			SendMessage(QP::System_HWND, WM_SETICON, ICON_BIG, (LPARAM)QP::SYSTEM_TITLE_ICON);
		}

		LONG Style = GetWindowLong(QP::System_HWND, GWL_STYLE);
		Style |= (WS_OVERLAPPEDWINDOW);
		SetWindowLong(QP::System_HWND, GWL_STYLE, Style);
		SetWindowPos(
			QP::System_HWND, NULL, 0, 0, 
			GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 
			SWP_NOZORDER | SWP_FRAMECHANGED
		);
		RedrawWindow(QP::System_HWND, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
		ShowWindow(QP::System_HWND, SW_MAXIMIZE);
	}

	void RenderFunc() {
		ScreenRect.Render(0.0, 0.0, QP::ViewportWidth, QP::ViewportHeight, 0.0, ScreenOpacity);

		if (SplashState)
			RenderSplashScreen();

		else {
			if (RenderLogo) {
				QP::Begin(RENDER_TYPE_STATIC);
				QP::Transform.RotateH(LogoRotation);
				QP::Transform.Scale(LogoSize, LogoSize);
				QP::ImageTool.SetLocalColor(1.0, 1.0, 1.0);
				QP::ImageTool.RenderImage(QP::SYSRES.MATA_LOGO, ScreenOpacity * LogoOpacity);

				ProgressLine.Render(-0.4, -0.3, 0.4, -0.3, 0.015, ScreenOpacity * ProgressBarOpacity * 0.3);
				ProgressLine.Render(-0.4, -0.3, -0.4 + (LoadingProgressLength / 5.0) * 0.8, -0.3, 0.015, ScreenOpacity * ProgressBarOpacity);
			}
		}
	}

	static DWORD WINAPI SystemResourceLoader(LPVOID Param) {
		QP::ImageTool.LoadImageT(QP::SYSRES.FMOD_LOGO, QP::SYSRES.FMOD_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
		QP::ImageTool.LoadImageT(QP::SYSRES.COLOR_TEXTURE, QP::SYSRES.COLOR_TEXTURE_DIRECTORY);

		QP::SYSRES.GLU_CIRCLE = gluNewQuadric();
		QP::SYSRES.GLU_LINE_CIRCLE = gluNewQuadric();

		gluQuadricDrawStyle(QP::SYSRES.GLU_CIRCLE, GLU_FILL);
		gluQuadricDrawStyle(QP::SYSRES.GLU_LINE_CIRCLE, GLU_FILL);

		return 0;
	}
};

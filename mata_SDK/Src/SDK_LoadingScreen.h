#pragma once
#include "SDK_Scene.h"
#include "SDK_FPSIndicator.h"

// Set work while loading here
void LoadingWork() {

}

#pragma region FoldRegion
class SDK_LoadingScreen : public SDK::Object {
private:
	bool    LoadStart{};

	SDK::ThreadHandle  SystemResourceLoadHandle{};
	SDK::ThreadHandle  ImageResourceLoadHandle{};
	SDK::ThreadHandle  SoundResourceLoadHandle{};
	SDK::ThreadHandle  DataResourceLoadHandle{};
	SDK::ThreadHandle  FontResourceLoadHandle{};

	bool    SystemResourceLoadEnd{};
	bool    ImageResourceLoadEnd{};
	bool    SoundResourceLoadEnd{};
	bool    DataResourceLoadEnd{};
	bool    FontResourceLoadEnd{};
	bool    InitializationEnd{};

	SDK::Timer        LoadingTimer{};
	SDK::Timer        StartTimer{};
	SDK::RectBrush    ScreenRect{};
	SDK::LineBrush    ProgressLine{};

	SDK::SoundChannel SndChannel{};
	bool              LogoSoundPlayed{};

	std::atomic<int>  LoadingProgress{};
	float             LoadingProgressLength{};
	float             ProgressBarOpacity{};

	float			  ScreenOpacity{ 1.0 };

	bool			  RenderLogo{};
	bool			  ExitState{};

public:
	SDK_LoadingScreen() {
		ScreenRect.SetColor(0.0, 0.0, 0.0);
		ScreenRect.SetRenderType(RENDER_TYPE_STATIC);
		ProgressLine.SetColor(1.0, 1.0, 1.0);
		ProgressLine.SetRenderType(RENDER_TYPE_STATIC);
	}

	void UpdateFunc(float FrameTime) {
		if (!LoadStart) {
			LoadingWork();

			SDK::Camera.Init();
			SDK::ImageTool.Init();
			SDK::SoundTool.Init();

			SDK::ImageTool.LoadImage(SDK::SYSRES.MATA_LOGO, SDK::SYSRES.MATA_LOGO_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
			SDK::ImageTool.LoadImage(SDK::SYSRES.SDK_LOGO, SDK::SYSRES.SDK_LOGO_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);

			ASSET::ResourcePreLoader();
			std::cout << "Resources pre-loaded." << std::endl;

			SDK::ThreadTool.Create(SystemResourceLoadHandle, SystemResourceLoader);

			LoadStart = true;
		}

		else {
			if (ENABLE_LOADING_SCREEN) {
				LoadingTimer.Update(FrameTime);
				if (LoadingTimer.CheckSec(1, CHECK_AND_RESUME)) {
					RenderLogo = true;
					SDK::SoundTool.PlayOnce(SDK::SYSRES.INTRO_SOUND, SndChannel, LogoSoundPlayed);
				}

				if (LoadingTimer.CheckSec(6, CHECK_AND_STOP)) {
					SDK::Math.Lerp(ProgressBarOpacity, 1.0, 5.0, FrameTime);
					SDK::Math.Lerp(LoadingProgressLength, (float)LoadingProgress, 5.0, FrameTime);

					if (LoadResources()) {
						if (!InitializationEnd) {
							SDK::ImageTool.Map();
							std::cout << "All of Image resources mapped." << std::endl;

							ModeAttribute();
							std::cout << "All of Mode pointers mapped." << std::endl;

							InitializationEnd = true;
						}

						else {
							if (LoadingProgress == 5) {
								if(StartTimer.UpdateAndCheckSec(1, CHECK_AND_STOP, FrameTime) && LoadingProgressLength >= 5.0) {
									if (!ExitState) {
										if (SHOW_FPS)  AddFPSIndicator();
										SDK::Scene.SwitchMode(SDK::START_MODE);
										ExitState = true;
									}

									else {
										ScreenOpacity -= FrameTime * 0.7;
										if (SDK::EXTool.CheckClampValue(ScreenOpacity, 0.0, CLAMP_LESS))
											SDK::Scene.DeleteObject(this);
									}
								}
							}
						}
					}
				}
			}

			else {
				if (LoadResources()) {
					if (!InitializationEnd) {
						SDK::ImageTool.Map();
						std::cout << "All of Image resources mapped." << std::endl;

						ModeAttribute();
						std::cout << "All of Mode pointers mapped." << std::endl;

						if (SHOW_FPS)  AddFPSIndicator();
						SDK::Scene.SwitchMode(SDK::START_MODE);
						SDK::Scene.DeleteObject(this);
						InitializationEnd = true;
					}
				}
			}
		}
	}

	void RenderFunc() {
		ScreenRect.Draw(0.0, 0.0, SDK::ViewportWidth, SDK::ViewportHeight, 0.0, ScreenOpacity);

		if (ENABLE_LOADING_SCREEN) {
			SDK::Begin(RENDER_TYPE_STATIC);
			SDK::Vector2 ImageSize = SDK::ImageTool.RealSize(SDK::SYSRES.SDK_LOGO);
			SDK::Transform.Move(SDK::WindowRect.LeftX, SDK::WindowRect.LeftY);
			SDK::Transform.Scale(0.4, 0.4);
			SDK::Transform.Move(ImageSize.x * 0.5, ImageSize.y * 0.5);
			SDK::ImageTool.SetLocalColor(0.2, 0.2, 0.2);
			SDK::ImageTool.RenderImage(SDK::SYSRES.SDK_LOGO, ScreenOpacity);

			if (RenderLogo) {
				SDK::Begin(RENDER_TYPE_STATIC);
				SDK::Transform.Scale(0.8, 0.8);
				SDK::ImageTool.SetLocalColor(1.0, 1.0, 1.0);
				SDK::ImageTool.RenderImage(SDK::SYSRES.MATA_LOGO, ScreenOpacity);

				ProgressLine.Draw(-0.6, -0.4, -0.6 + (LoadingProgressLength / 5.0) * 1.2, -0.4, 0.02, ScreenOpacity * ProgressBarOpacity);
			}
		}
	}

	void AddFPSIndicator() {
		SDK::Scene.AddSystemObject(new SDK_FPS_Indicator);
	}

	bool LoadResources() {
		if (!SystemResourceLoadEnd && !SDK::ThreadTool.CheckRunning(SystemResourceLoadHandle)) {
			SDK::ThreadTool.Close(SystemResourceLoadHandle);
			SDK::ThreadTool.Create(ImageResourceLoadHandle, ASSET::ImageResourceLoader);
			SDK::ThreadTool.Create(SoundResourceLoadHandle, ASSET::SoundResourceLoader);
			SDK::ThreadTool.Create(DataResourceLoadHandle, ASSET::FileResourceLoader);
			SDK::ThreadTool.Create(FontResourceLoadHandle, ASSET::FontResourceLoader);
			std::cout << "System resource load completed." << std::endl;

			LoadingProgress.fetch_add(1);
			SystemResourceLoadEnd = true;
		}

		if (!ImageResourceLoadEnd && !SDK::ThreadTool.CheckRunning(ImageResourceLoadHandle)) {
			SDK::ThreadTool.Close(ImageResourceLoadHandle);
			std::cout << "Image resource load completed." << std::endl;

			LoadingProgress.fetch_add(1);
			ImageResourceLoadEnd = true;
		}

		if (!SoundResourceLoadEnd && !SDK::ThreadTool.CheckRunning(SoundResourceLoadHandle)) {
			SDK::ThreadTool.Close(SoundResourceLoadHandle);
			std::cout << "Sound resource load completed." << std::endl;

			LoadingProgress.fetch_add(1);
			SoundResourceLoadEnd = true;
		}

		if (!DataResourceLoadEnd && !SDK::ThreadTool.CheckRunning(DataResourceLoadHandle)) {
			SDK::ThreadTool.Close(DataResourceLoadHandle);
			std::cout << "Data resource load completed." << std::endl;

			LoadingProgress.fetch_add(1);
			DataResourceLoadEnd = true;
		}

		if (!FontResourceLoadEnd && !SDK::ThreadTool.CheckRunning(FontResourceLoadHandle)) {
			SDK::ThreadTool.Close(FontResourceLoadHandle);
			std::cout << "Font resource load completed." << std::endl;

			SDK::SYSRES.SYSTEM_FONT_REGULAR.Init(L"Space Grotesk");
			SDK::SYSRES.SYSTEM_FONT_BOLD.Init(L"Space Grotesk", FW_BOLD);

			ASSET::FontResourceInitializer();
			std::cout << "Font resources created." << std::endl;

			LoadingProgress.fetch_add(1);
			FontResourceLoadEnd = true;
		}

		if (SystemResourceLoadEnd && ImageResourceLoadEnd && SoundResourceLoadEnd && DataResourceLoadEnd && FontResourceLoadEnd)
			return true;

		return false;
	}

	static DWORD WINAPI SystemResourceLoader(LPVOID Param) {
		SDK::ImageTool.LoadImageT(SDK::SYSRES.FMOD_LOGO, SDK::SYSRES.FMOD_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
		SDK::ImageTool.LoadImageT(SDK::SYSRES.COLOR_TEXTURE, SDK::SYSRES.COLOR_TEXTURE_DIRECTORY);
		SDK::SoundTool.Load(SDK::SYSRES.INTRO_SOUND, SDK::SYSRES.SDK_LOGO_SOUND_DIRECTORY);
		SDK::FontLoader.Load(SDK::SYSRES.SDK_FONT_DIRECTORY);

		SDK::SYSRES.GLU_CIRCLE = gluNewQuadric();
		SDK::SYSRES.GLU_LINE_CIRCLE = gluNewQuadric();
		gluQuadricDrawStyle(SDK::SYSRES.GLU_CIRCLE, GLU_FILL);
		gluQuadricDrawStyle(SDK::SYSRES.GLU_LINE_CIRCLE, GLU_FILL);
		return 0;
	}
};
#pragma endregion

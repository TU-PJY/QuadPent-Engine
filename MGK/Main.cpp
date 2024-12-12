#include "EngineHeader.h"
#include "CameraUtil.h"
#include "CameraController.h"
#include "Scene.h"
#include "TextUtil.h"
#include "RectBrush.h"

#ifdef USE_SOUND_SYSTEM
#include "SoundUtil.h"
#endif

#include <thread>
#include <chrono>

bool UpdateActivateCommand;

// frametime values
float PrevTime, CurrentTime, DeltaTime;

// FPS indicator
RectBrush Rect;
TextUtil FPS_IND;
TimerUtil FPS_IND_REFRESH_TIMER;
glm::vec2 RectPosition;
float CurrentDeltaTime = 1.0;

GLvoid Framework::Framework() {
	glClearColor(BackColor.r, BackColor.g, BackColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (UpdateActivateCommand) {
		scene.Routine();
		scene.CompleteCommand();
		cameraCon.Update(DeltaTime);
#ifdef USE_SOUND_SYSTEM
		soundUtil.Update();
#endif

		if (SHOW_FPS) {
			FPS_IND_REFRESH_TIMER.Update(DeltaTime);
			if (FPS_IND_REFRESH_TIMER.CheckMiliSec(0.5, 1, CHECK_AND_INTERPOLATE))
				CurrentDeltaTime = DeltaTime;

			UI::ClampPositionToCorner(EDGE_LEFT_UP, RectPosition.x, RectPosition.y, 0.25, 0.08, 0.0, 0.0);
			Rect.Draw(RectPosition.x, RectPosition.y, 0.25, 0.08, 0.0, 0.3);
			FPS_IND.Render(WindowRect.lx + 0.01, WindowRect.ry, 0.05, L"FPS: %d", (int)(1.0 / CurrentDeltaTime));
		}
	}

	CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	
	if(FPSLimit > 0)
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast <int>(DestFPS)));

	DeltaTime = (CurrentTime - PrevTime) / 1000.0;
	scene.InputFrameTime(DeltaTime);

	PrevTime = CurrentTime;

	glutSwapBuffers();
	glutPostRedisplay();

	UpdateActivateCommand = true;
}

void main(int argc, char** argv) {
	Framework::SetupSystem(argc, argv);

	if (SHOW_FPS) {
		FPS_IND.Init(L"Arial", FW_NORMAL);
		FPS_IND.SetColor(1.0, 1.0, 1.0);
		FPS_IND.SetHeightAlign(HEIGHT_ALIGN_UNDER);
	}

	glutDisplayFunc(Framework::Framework);
	glutReshapeFunc(Framework::DisplayReshape);
	glutMainLoop();
}

void Framework::Exit() {
	glutDestroyWindow(1);
}

GLfloat ASP(GLfloat Value) {
	return Value * ASPECT;
}
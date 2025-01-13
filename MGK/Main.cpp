#include "EngineHeader.h"
#include "CameraController.h"
#include "FPSInd.h"
#include "Scene.h"

#ifdef USE_SOUND_SYSTEM
#include "SoundUtil.h"
#endif

#include <thread>
#include <chrono>

bool UpdateActivateCommand;

// frametime values
float PrevTime, CurrentTime, DeltaTime;

GLvoid System::Main() {
	if (FPSLimit > 0)
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast <int>(DestFPS)));

	glClearColor(BackColor.r, BackColor.g, BackColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (UpdateActivateCommand) {
		scene.Update();
		scene.Render();
		scene.CompleteCommand();
#ifdef USE_SOUND_SYSTEM
		soundUtil.Update();
#endif
	}

	if (SHOW_FPS && Indicator)
		Indicator->RenderIndicator();

	CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	DeltaTime = (CurrentTime - PrevTime) / 1000.0;
	scene.InputFrameTime(DeltaTime);

	PrevTime = CurrentTime;

	glutSwapBuffers();
	glutPostRedisplay();

	UpdateActivateCommand = true;
}

void main(int argc, char** argv) {
	if (SHOW_CONSOLE) {
		if (AllocConsole()) {
			FILE* FP{};

			freopen_s(&FP, "CONOUT$", "w", stdout);
			freopen_s(&FP, "CONIN$", "r", stdin);
			freopen_s(&FP, "CONERR$", "w", stderr);

			std::cout << "Console initialized successfully." << std::endl;
		}
	}

	System::SetupSystem(argc, argv);
	glutDisplayFunc(System::Main);
	glutReshapeFunc(System::DisplayReshape);
	glutMainLoop();
}

void System::Exit() {
	glutDestroyWindow(1);
}

GLfloat ASP(GLfloat Value) {
	return Value * ASPECT;
}
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

GLvoid Framework::Framework() {
	if (FPSLimit > 0)
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast <int>(DestFPS)));

	glClearColor(BackColor.r, BackColor.g, BackColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (UpdateActivateCommand) {
		scene.Routine();
		scene.CompleteCommand();
		CameraControl->UpdateCamera(DeltaTime);
#ifdef USE_SOUND_SYSTEM
		soundUtil.Update();
#endif
	}

	CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	DeltaTime = (CurrentTime - PrevTime) / 1000.0;
	scene.InputFrameTime(DeltaTime);

	PrevTime = CurrentTime;

	glutSwapBuffers();
	glutPostRedisplay();

	UpdateActivateCommand = true;
}

void main(int argc, char** argv) {
	Framework::SetupSystem(argc, argv);
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
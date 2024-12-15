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
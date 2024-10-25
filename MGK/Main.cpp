#include "EngineHeader.h"
#include "CameraUtil.h"
#include "CameraController.h"
#include "Scene.h"
#include "SoundUtil.h"
#include <thread>
#include <chrono>

bool UpdateActivateCommand;

// frametime values
float PrevTime, CurrentTime, DeltaTime;

GLvoid Framework() {
	glClearColor(BackColor.r, BackColor.g, BackColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (UpdateActivateCommand) {
		scene.Routine();
		cameraCon.Update(DeltaTime);
		soundUtil.Update();
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
	SetupSystem(argc, argv);
	glutDisplayFunc(Framework);
	glutReshapeFunc(DisplayReshape);
	glutMainLoop();
}
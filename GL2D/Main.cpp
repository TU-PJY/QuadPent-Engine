#include "EngineHeader.h"
#include "Framework.h"
#include "CameraUtil.h"
#include "SoundUtil.h"

// frametime values
float PrevTime, CurrentTime, DeltaTime;

GLvoid GLMain() {
	glClearColor(BackColor.r, BackColor.g, BackColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	fw.Routine();
	soundUtil.Update();

	CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	DeltaTime = (CurrentTime - PrevTime) / 1000.0;

	fw.InputFrameTime(DeltaTime);

	PrevTime = CurrentTime;

	glutSwapBuffers();
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	SetupSystem(argc, argv);
	InitSystem();

	glutDisplayFunc(GLMain);
	glutReshapeFunc(DisplayReshape);
	glutMainLoop();
}
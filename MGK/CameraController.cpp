#include "CameraController.h"
#include "CameraUtil.h"
#include "TransformUtil.h"

void CameraController::InputKey(KeyEvent& Event) {

}

void CameraController::UpdateFunc(float FT){
	// add logic here
	ComputeCameraMatrix();
}

void CameraController::MoveCamera(GLfloat X, GLfloat Y){
	Position.x = X;
	Position.y = Y;
}

void CameraController::MoveCamera(glm::vec2& PositionValue){
	Position = PositionValue;
}

void CameraController::RotateCamera(GLfloat Degree){
	Rotation = Degree;
}

void CameraController::CameraZoom(int ZoomType, GLfloat ZoomValue){
	switch (ZoomType) {
	case ZOOM_IN:
		camera.ZoomValue = camera.ZoomValue / (1.0f - ZoomValue);
		break;

	case ZOOM_OUT:
		camera.ZoomValue = camera.ZoomValue * (1.0f - ZoomValue);
		break;
	}
}

void CameraController::ChangeCameraZoom(GLfloat ZoomValue){
	camera.ZoomValue = ZoomValue;
}

GLfloat CameraController::ComputeNextZoom(int ZoomType, GLfloat ZoomValue) {
	if (ZoomType == ZOOM_IN)
		return camera.ZoomValue / (1.0f - ZoomValue);
	else if (ZoomType == ZOOM_OUT)
		return camera.ZoomValue * (1.0f - ZoomValue);
	else
		return camera.ZoomValue;
}

///////////////////////////////////////// private

void CameraController::ComputeCameraMatrix(){
	transform.Identity(camera.TranslateMatrix);
	transform.Identity(camera.RotateMatrix);
	transform.Move(camera.TranslateMatrix, Position.x, Position.y);
	transform.Rotate(camera.RotateMatrix, Rotation);
}
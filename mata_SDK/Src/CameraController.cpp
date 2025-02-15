#include "CameraController.h"
#include "CameraUtil.h"
#include "TransformUtil.h"
#include "AnimationUtil.h"

CameraController SDK::CameraControl;
glm::vec2 SDK::CameraPosition;
GLfloat SDK::CameraRotation;
GLfloat SDK::CameraZoom = 1.0;

void CameraController::Update(float FrameTime){
	// add logic here

	ComputeCameraMatrix();
}

void CameraController::Move(GLfloat X, GLfloat Y){
	Position.x = -X;
	Position.y = -Y;
	SDK::CameraPosition.x = X;
	SDK::CameraPosition.y = Y;
}

void CameraController::Move(glm::vec2& PositionValue){
	Position = -PositionValue;
	SDK::CameraPosition = PositionValue;
}

void CameraController::Rotate(GLfloat Degree){
	Rotation = -Degree;
	SDK::CameraRotation = Degree;
}

void CameraController::Zoom(int ZoomType, GLfloat ZoomValue){
	switch (ZoomType) {
	case ZOOM_IN:
		camera.ZoomValue = camera.ZoomValue / (1.0f - ZoomValue);
		break;

	case ZOOM_OUT:
		camera.ZoomValue = camera.ZoomValue * (1.0f - ZoomValue);
		break;
	}

	SDK::CameraZoom = camera.ZoomValue;
}

void CameraController::SetZoom(GLfloat ZoomValue){
	camera.ZoomValue = ZoomValue;
	SDK::CameraZoom = camera.ZoomValue;
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
	transform.Identity(camera.CameraMatrix);
	transform.Rotate(camera.CameraMatrix, Rotation);
	transform.Move(camera.CameraMatrix, Position);
}
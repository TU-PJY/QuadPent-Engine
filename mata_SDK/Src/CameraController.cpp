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
		SDK::Camera.ZoomValue = SDK::Camera.ZoomValue / (1.0f - ZoomValue);
		break;

	case ZOOM_OUT:
		SDK::Camera.ZoomValue = SDK::Camera.ZoomValue * (1.0f - ZoomValue);
		break;
	}

	SDK::CameraZoom = SDK::Camera.ZoomValue;
}

void CameraController::SetZoom(GLfloat ZoomValue){
	SDK::Camera.ZoomValue = ZoomValue;
	SDK::CameraZoom = SDK::Camera.ZoomValue;
}

GLfloat CameraController::ComputeNextZoom(int ZoomType, GLfloat ZoomValue) {
	if (ZoomType == ZOOM_IN)
		return 	SDK::Camera.ZoomValue / (1.0f - ZoomValue);
	else if (ZoomType == ZOOM_OUT)
		return 	SDK::Camera.ZoomValue * (1.0f - ZoomValue);
	else
		return 	SDK::Camera.ZoomValue;
}

///////////////////////////////////////// private

void CameraController::ComputeCameraMatrix(){
	SDK::Transform.Identity(SDK::Camera.CameraMatrix);
	SDK::Transform.Rotate(SDK::Camera.CameraMatrix, Rotation);
	SDK::Transform.Move(SDK::Camera.CameraMatrix, Position);
}
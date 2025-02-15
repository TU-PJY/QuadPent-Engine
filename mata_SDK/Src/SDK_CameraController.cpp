#include "SDK_CameraController.h"
#include "SDK_Camera.h"
#include "SDK_Transform.h"
#include "SDK_AnimationTool.h"

SDK_CameraController SDK::CameraControl;

void SDK_CameraController::Update(float FrameTime){
	// add logic here

	ComputeCameraMatrix();
}

void SDK_CameraController::Move(GLfloat X, GLfloat Y){
	Position.x = -X;
	Position.y = -Y;
	SDK::Camera.Position.x = X;
	SDK::Camera.Position.y = Y;
}

void SDK_CameraController::Move(glm::vec2& PositionValue){
	Position = -PositionValue;
	SDK::Camera.Position = PositionValue;
}

void SDK_CameraController::Rotate(GLfloat Degree){
	Rotation = -Degree;
	SDK::Camera.Rotation = Degree;
}

void SDK_CameraController::Zoom(int ZoomType, GLfloat ZoomValue){
	switch (ZoomType) {
	case ZOOM_IN:
		SDK::Camera.Zoom = SDK::Camera.Zoom / (1.0f - ZoomValue);
		break;

	case ZOOM_OUT:
		SDK::Camera.Zoom = SDK::Camera.Zoom * (1.0f - ZoomValue);
		break;
	}
}

void SDK_CameraController::SetZoom(GLfloat ZoomValue){
	SDK::Camera.Zoom = ZoomValue;
}

GLfloat SDK_CameraController::ComputeNextZoom(int ZoomType, GLfloat ZoomValue) {
	if (ZoomType == ZOOM_IN)
		return 	SDK::Camera.Zoom / (1.0f - ZoomValue);
	else if (ZoomType == ZOOM_OUT)
		return 	SDK::Camera.Zoom * (1.0f - ZoomValue);
	else
		return 	SDK::Camera.Zoom;
}

///////////////////////////////////////// private

void SDK_CameraController::ComputeCameraMatrix(){
	SDK::Transform.Identity(SDK::Camera.CameraMatrix);
	SDK::Transform.Rotate(SDK::Camera.CameraMatrix, Rotation);
	SDK::Transform.Move(SDK::Camera.CameraMatrix, Position);
}
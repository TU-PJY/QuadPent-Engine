#include "SDK_CameraController.h"
#include "SDK_Camera.h"
#include "SDK_Transform.h"
#include "SDK_AnimationTool.h"

SDK::SDK_CameraController SDK::CameraControl;

void SDK::SDK_CameraController::Update(float FrameTime){
	// add logic here
	ComputeCameraMatrix();
}

void SDK::SDK_CameraController::Move(float X, float Y){
	Position.x = -X;
	Position.y = -Y;
	SDK::Camera.Position.x = X;
	SDK::Camera.Position.y = Y;
}

void SDK::SDK_CameraController::Move(SDK::Vector2& PositionValue){
	Position = -PositionValue;
	SDK::Camera.Position = PositionValue;
}

void SDK::SDK_CameraController::Rotate(float Degree){
	Rotation = Degree;
	SDK::Camera.Rotation = Degree;
}

void SDK::SDK_CameraController::Zoom(int ZoomType, float ZoomValue){
	switch (ZoomType) {
	case ZOOM_IN:
		ControllerZoom = ControllerZoom / (1.0f - ZoomValue);
		break;

	case ZOOM_OUT:
		ControllerZoom = ControllerZoom * (1.0f - ZoomValue);
		break;
	}
}

void SDK::SDK_CameraController::SetZoom(float ZoomValue){
	ControllerZoom = ZoomValue;
}

float SDK::SDK_CameraController::ComputeNextZoom(int ZoomType, float ZoomValue) {
	if (ZoomType == ZOOM_IN)
		return 	ControllerZoom / (1.0f - ZoomValue);
	else if (ZoomType == ZOOM_OUT)
		return 	ControllerZoom * (1.0f - ZoomValue);
	else
		return 	ControllerZoom;
}

///////////////////////////////////////// private

void SDK::SDK_CameraController::ComputeCameraMatrix(){
	SDK::Transform.Identity(SDK::Camera.CameraMatrix);
	SDK::Camera.Zoom = ControllerZoom;
	SDK::Camera.CameraMatrix = rotate(SDK::Camera.CameraMatrix, glm::radians(Rotation), SDK::Vector3(0.0, 0.0, 1.0));
	SDK::Camera.CameraMatrix = translate(SDK::Camera.CameraMatrix, SDK::Vector3(Position.x, Position.y, 0.0));
}
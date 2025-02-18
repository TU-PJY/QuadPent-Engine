#include "SDK_CameraController.h"
#include "SDK_Camera.h"
#include "SDK_Transform.h"
#include "SDK_AnimationTool.h"

MSDK::SDK_CameraController MSDK::CameraControl;

void MSDK::SDK_CameraController::Update(float FrameTime){
	// add logic here

	ComputeCameraMatrix();
}

void MSDK::SDK_CameraController::Move(float X, float Y){
	Position.x = -X;
	Position.y = -Y;
	MSDK::Camera.Position.x = X;
	MSDK::Camera.Position.y = Y;
}

void MSDK::SDK_CameraController::Move(glm::vec2& PositionValue){
	Position = -PositionValue;
	MSDK::Camera.Position = PositionValue;
}

void MSDK::SDK_CameraController::Rotate(float Degree){
	Rotation = -Degree;
	MSDK::Camera.Rotation = Degree;
}

void MSDK::SDK_CameraController::Zoom(int ZoomType, float ZoomValue){
	switch (ZoomType) {
	case ZOOM_IN:
		MSDK::Camera.Zoom = MSDK::Camera.Zoom / (1.0f - ZoomValue);
		break;

	case ZOOM_OUT:
		MSDK::Camera.Zoom = MSDK::Camera.Zoom * (1.0f - ZoomValue);
		break;
	}
}

void MSDK::SDK_CameraController::SetZoom(float ZoomValue){
	MSDK::Camera.Zoom = ZoomValue;
}

float MSDK::SDK_CameraController::ComputeNextZoom(int ZoomType, float ZoomValue) {
	if (ZoomType == ZOOM_IN)
		return 	MSDK::Camera.Zoom / (1.0f - ZoomValue);
	else if (ZoomType == ZOOM_OUT)
		return 	MSDK::Camera.Zoom * (1.0f - ZoomValue);
	else
		return 	MSDK::Camera.Zoom;
}

///////////////////////////////////////// private

void MSDK::SDK_CameraController::ComputeCameraMatrix(){
	MSDK::Transform.Identity(MSDK::Camera.CameraMatrix);
	MSDK::Transform.Rotate(MSDK::Camera.CameraMatrix, Rotation);
	MSDK::Transform.Move(MSDK::Camera.CameraMatrix, Position);
}
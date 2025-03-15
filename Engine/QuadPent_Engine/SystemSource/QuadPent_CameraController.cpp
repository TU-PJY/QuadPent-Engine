#include "QuadPent_CameraController.h"
#include "QuadPent_Camera.h"
#include "QuadPent_Transform.h"
#include "QuadPent_AnimationTool.h"

QP::QuadPent_CameraController QP::CameraControl;

void QP::QuadPent_CameraController::Update(){
	ComputeCameraMatrix();
}

void QP::QuadPent_CameraController::Move(float X, float Y){
	Position.x = -X;
	Position.y = -Y;
	QP::Camera.Position.x = X;
	QP::Camera.Position.y = Y;
}

void QP::QuadPent_CameraController::Move(QP::Vector2& PositionValue){
	Position = -PositionValue;
	QP::Camera.Position = PositionValue;
}

void QP::QuadPent_CameraController::Rotate(float Degree){
	Rotation = Degree;
	QP::Camera.Rotation = Degree;
}

void QP::QuadPent_CameraController::Zoom(int ZoomType, float ZoomValue){
	switch (ZoomType) {
	case ZOOM_IN:
		ControllerZoom = ControllerZoom / (1.0f - ZoomValue);
		break;

	case ZOOM_OUT:
		ControllerZoom = ControllerZoom * (1.0f - ZoomValue);
		break;
	}
}

void QP::QuadPent_CameraController::SetZoom(float ZoomValue){
	ControllerZoom = ZoomValue;
}

float QP::QuadPent_CameraController::ComputeNextZoom(int ZoomType, float ZoomValue) {
	if (ZoomType == ZOOM_IN)
		return 	ControllerZoom / (1.0f - ZoomValue);
	else if (ZoomType == ZOOM_OUT)
		return 	ControllerZoom * (1.0f - ZoomValue);
	else
		return 	ControllerZoom;
}

///////////////////////////////////////// private

void QP::QuadPent_CameraController::ComputeCameraMatrix(){
	QP::Transform.Identity(QP::Camera.CameraMatrix);
	QP::Camera.Zoom = ControllerZoom;
	QP::Camera.CameraMatrix = rotate(QP::Camera.CameraMatrix, glm::radians(Rotation), QP::Vector3(0.0, 0.0, 1.0));
	QP::Camera.CameraMatrix = translate(QP::Camera.CameraMatrix, QP::Vector3(Position.x, Position.y, 0.0));
}
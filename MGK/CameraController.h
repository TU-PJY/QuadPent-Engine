#pragma once
#include "EngineHeader.h"

enum ZoomType
{ ZOOM_IN, ZOOM_OUT };

class CameraController {
public:
	glm::vec2 Position{};
	GLfloat Rotation{};

	// Update camera variables.
	void Update(float FT);

	// Apply the updated values ​​to the camera matrix.
	void CalcMatrix();

	// Move the camera position.
	void Move(GLfloat X, GLfloat Y);

	// Move the camera position.
	void Move(glm::vec2& PositionValue);

	// Rotate the camera.
	void Rotate(GLfloat Value);

	// Adjust camera zoom. The zoom value changes according to the entered value.
	void Zoom(int Type, GLfloat Value);

	// Changes the camera zoom to a specific number.
	void ChangeZoom(GLfloat Value);

	// Returns the zoom numerical change value according to the entered value.
	GLfloat CalculateNextZoom(int Type, GLfloat Value);
};
extern CameraController camController;
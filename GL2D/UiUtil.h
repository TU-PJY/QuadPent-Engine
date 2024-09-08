#pragma once
#include "EngineHeader.h"

enum class Edge 
{ LeftDown, LeftUp, RightDown, RightUp };

class UiUtil {
public:
	void ClampToCorner(Edge EdgePosition, glm::mat4& Matrix, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY);
};
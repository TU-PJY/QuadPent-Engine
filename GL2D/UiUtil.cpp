#include "UiUtil.h"

void UiUtil::ClampToCorner(Edge EdgePosition, glm::mat4& Matrix, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	switch (EdgePosition) {
	case Edge::LeftDown:
		Matrix = translate(Matrix, glm::vec3(rect.lx + Width / 2.0 + GapX, rect.ly + Height / 2.0 + GapY, 0.0));
		break;

	case Edge::LeftUp:
		Matrix = translate(Matrix, glm::vec3(rect.lx + Width / 2.0 + GapX, rect.ry - Height / 2.0 - GapY, 0.0));
		break;

	case Edge::RightDown:
		Matrix = translate(Matrix, glm::vec3(rect.rx - Width / 2.0 - GapX, rect.ly + Height / 2.0 + GapY, 0.0));
		break;

	case Edge::RightUp:
		Matrix = translate(Matrix, glm::vec3(rect.rx - Width / 2.0 - GapX, rect.ry - Height / 2.0 - GapY, 0.0));
		break;
	}
}
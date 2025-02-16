#include "SDK_ObjectValue.h"

// Object Value
glm::mat4 MoveMatrix	    = glm::mat4(1.0);
glm::mat4 RotateMatrix		= glm::mat4(1.0);
glm::mat4 ScaleMatrix		= glm::mat4(1.0);
glm::mat4 FlipMatrix        = glm::mat4(1.0);
glm::mat4 ImageAspectMatrix = glm::mat4(1.0);
glm::mat4 ResultMatrix      = glm::mat4(1.0);

glm::mat4 UnitMoveMatrix    = glm::mat4(1.0);
glm::mat4 UnitRotateMatrix  = glm::mat4(1.0);
glm::mat4 UnitScaleMatrix   = glm::mat4(1.0);
glm::mat4 UnitFlipMatrix    = glm::mat4(1.0);

glm::mat4 ViewportPositionMatrix = glm::mat4(1.0);

glm::vec3 ObjectColor;

GLfloat ObjectOpacityValue;
GLfloat ObjectBlurValue;

GLfloat UnitBlurValue;
GLfloat UnitOpacityValue;
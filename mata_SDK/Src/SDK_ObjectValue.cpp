#include "SDK_ObjectValue.h"

// Object Value
glm::mat4 MSDK::MoveMatrix	     = glm::mat4(1.0);
glm::mat4 MSDK::RotateMatrix		 = glm::mat4(1.0);
glm::mat4 MSDK::ScaleMatrix		 = glm::mat4(1.0);
glm::mat4 MSDK::FlipMatrix        = glm::mat4(1.0);
glm::mat4 MSDK::ResultMatrix      = glm::mat4(1.0);

glm::mat4 MSDK::UnitMoveMatrix    = glm::mat4(1.0);
glm::mat4 MSDK::UnitRotateMatrix  = glm::mat4(1.0);
glm::mat4 MSDK::UnitScaleMatrix   = glm::mat4(1.0);
glm::mat4 MSDK::UnitFlipMatrix    = glm::mat4(1.0);

glm::mat4 MSDK::ViewportPositionMatrix = glm::mat4(1.0);

glm::vec3 MSDK::ObjectColor;

float MSDK::ObjectOpacityValue;
float MSDK::ObjectBlurValue;

float MSDK::UnitBlurValue;
float MSDK::UnitOpacityValue;
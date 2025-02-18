#include "SDK_ObjectValue.h"

// Object Value
glm::mat4 SDK::MoveMatrix	     = glm::mat4(1.0);
glm::mat4 SDK::RotateMatrix		 = glm::mat4(1.0);
glm::mat4 SDK::ScaleMatrix		 = glm::mat4(1.0);
glm::mat4 SDK::FlipMatrix        = glm::mat4(1.0);
glm::mat4 SDK::ResultMatrix      = glm::mat4(1.0);

glm::mat4 SDK::UnitMoveMatrix    = glm::mat4(1.0);
glm::mat4 SDK::UnitRotateMatrix  = glm::mat4(1.0);
glm::mat4 SDK::UnitScaleMatrix   = glm::mat4(1.0);
glm::mat4 SDK::UnitFlipMatrix    = glm::mat4(1.0);

glm::mat4 SDK::ViewportPositionMatrix = glm::mat4(1.0);

glm::vec3 SDK::ObjectColor;

float SDK::ObjectOpacityValue;
float SDK::ObjectBlurValue;

float SDK::UnitBlurValue;
float SDK::UnitOpacityValue;